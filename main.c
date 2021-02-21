
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include "SDL_terminal.h"

#include "rmk8085/8080emu.h"
#include "rmk8085/pe_base.h"
#include "rmk8085/pe_sram.h"
#include "rmk8085/pe_i8155.h"
#include "rmk8085/pe_bq3285.h"
#include "rmk8085/pe_i8251.h"
#include "rmk8085/pe_rom.h"
#include "rmk8085/pe_interaction.h"

pe_rom_t rom1;
pe_rom_t rom2;
pe_rom_t rom3;
pe_rom_t rom4;
pe_i8155_t i8155_f800;
pe_i8155_t i8155_b800;
pe_sram_64k_t sram_main;
pe_bq3285_t rtc;
pe_i8251_t uart;


State8080	cpu;
uint8_t		rmk_mode = 0xe0;

volatile bool close_app = false;

int callback_on_new_char(uint8_t new_char, void * userdata);
int callback_on_fetch_char(uint8_t *new_char, void * userdata);
int callback_on_kbhit_char(bool *have_data, void * userdata);
uint8_t cpu_in(uint16_t port)
{
	return pe_io_read(port);
}

void cpu_out(uint16_t port, uint8_t data)
{
	switch (port)
	{
		case 0xE0:
		case 0xE1:
		//case 0x77: // ???
		{
			/* ROM present in 0x0000 thru 0x0FFF */
			/* Backup user ram */
			//memcpy(shadow_ram, state->memory, 0x1000);
			//memcpy(shadow_ram, sram_main._sram, 0x1000);
			rom1.base.enabled = true;
			/* Put the rom again there */
			//ReadFileIntoMemoryAt("gmon.bin", 0);

			rmk_mode = port;

			//printf("! ROM selected, PC = 0x%.4x, mode = %.2x\n", cpu.pc, port);
			break;
		}
		case 0xE2:
		case 0xE3:
		//case 0x7E: // ???
		{
			/* RAM present in 0x0000 thru 0x0FFF */
			/* Reload user ram */
			//memcpy(state->memory, shadow_ram, 0x1000);
			//memcpy(sram_main._sram, shadow_ram, 0x1000);
			rom1.base.enabled = false;

			rmk_mode = port;

			//printf("! RAM selected, PC = 0x%.4x, mode = %.2x\n", cpu.pc, port);
			break;
		}
		default:
		{
			pe_io_write(port, data);
		}
	}
}

void cpu_mem_write(uint16_t address, uint8_t value)
{
	if (rmk_mode == 0xE2 || rmk_mode == 0xE3)
	{
		//printf("* RAM Write in %.2x mode: %.2x <- %.4x in PC = %.4x\n", rmk_mode, value, address, state->pc);
	}
	pe_mem_write(address, value);
}

uint8_t cpu_mem_read(uint16_t address)
{
	return pe_mem_read(address);
}

int rmk_run(State8080 * state)
{
	unsigned char	op;
	int				cycles;

	//op = &state->memory[state->pc];
	/*sram_main.base.mread(state->pc, &op, &PE_AS_BASE(sram_main));

	if (state->pc == 0x014b)
	{
		//cycle_count = cycle_count;
	}
	if (state->pc == 0x0)
	{
		//cycle_count = cycle_count;
	}
	if (op == 0xdb) //machine specific handling for IN
	{
		//sram_main.base.mread(state->pc + 1, &op, &PE_AS_BASE(sram_main));
		//state->a = cpu_in(state, op);
	}
	else if (op == 0xd3) //machine specific handling for OUT
	{
		//sram_main.base.mread(state->pc + 1, &op, &PE_AS_BASE(sram_main));
		//cpu_out(state, op, state->a);
	}*/

	op = state->mem_read(state->pc);

	/* Debug thing of the RMK board */
	if ((rmk_mode == 0xE2 && ((op & 0xC0) == 0x40) && ((op >> 3) & 7) == (op & 7)) ||
		(rmk_mode == 0xE3))
	{
		if (rmk_mode == 0xE3)
		{
			printf("\n# DBG: detected mode 0xE3 (opcode %.2x), SS (sleeping for 2), TRAP!\n", op);
			//sleep(2);
		}
		else
		{
			printf("\n# DBG: detected debug opcode %.2x at 0x%.4x, TRAP!\n", op, state->pc);
		}

		GenerateTrap(state);
		cpu_out(0xE0, 0);
	}

	cycles = Emulate8080Op(state);


	return cycles;
}

pe_base1_t * pe_list[] =
{
	&PE_AS_BASE(uart),
	&PE_AS_BASE(i8155_f800),
	&PE_AS_BASE(i8155_b800),
	&PE_AS_BASE(rtc),
	&PE_AS_BASE(rom1),
	&PE_AS_BASE(rom2),
	&PE_AS_BASE(rom3),
	&PE_AS_BASE(rom4),
	&PE_AS_BASE(sram_main),
};

void pe_start(SDL_Terminal *terminal)
{
	FILE *fh;

	/* Monitor ROM */
	fh = fopen("monitor.bin", "rb");
	//fh = fopen("gmon.bin", "rb");
	rom1.base.mem_start = 0x0000;
	rom1.base.mem_end = (4*1024) - 1 + rom1.base.mem_start;
	rom1.base.enabled = fh ? true : false;
	pe_rom_init(&rom1, fh, 0);

	/* Monitor Cont */
	fh = fopen("monitor_cont.bin", "rb");
	rom4.base.mem_start = 0xE000;
	rom4.base.mem_end = (4*1024) - 1 + rom4.base.mem_start;
	rom4.base.enabled = fh ? true : false;
	pe_rom_init(&rom4, fh, 0);

	/* Editor 0 */
	fh = fopen("editor0.bin", "rb");
	rom2.base.mem_start = 0x8000;
	rom2.base.mem_end = (4*1024) - 1 + rom2.base.mem_start;
	rom2.base.enabled = fh ? true : false;
	pe_rom_init(&rom2, fh, 0);

	/* Editor 1 */
	fh = fopen("editor1.bin", "rb");
	rom3.base.mem_start = 0x9000;
	rom3.base.mem_end = (4*1024) - 1 + rom3.base.mem_start;
	rom3.base.enabled = fh ? true : false;
	pe_rom_init(&rom3, fh, 0);

	/* Main SRAM */
	sram_main.base.mem_start = 0;
	sram_main.base.mem_end = (64*1024) - 1;
	sram_main.base.enabled = true;
	pe_sram_init(&PE_SRAM_AS_BASE(sram_main), 64*1024);

	/* i8155 @ level 1 */
	i8155_f800.base.mem_start = 0xF800;
	i8155_f800.base.mem_end = (64*1024) - 1;
	i8155_f800.base.io_start = 0xF8;
	i8155_f800.base.io_end = 0xFF;
	i8155_f800.base.enabled = true;
	pe_i8155_init(&i8155_f800);

	/* i8155 @ level 2 (RI/0-1) */
	i8155_b800.base.mem_start = 0xB800;
	i8155_b800.base.mem_end = 0xC000 - 1;
	i8155_b800.base.io_start = 0xB8;
	i8155_b800.base.io_end = 0xBF;
	i8155_b800.base.enabled = true;
	pe_i8155_init(&i8155_b800);

	/* RTC (taking the place of the RI/0-0) @ level 2 */
	rtc.base.mem_start = 0xB000;
	rtc.base.mem_end = 0xB7FF;
	rtc.base.enabled = true;
	pe_bq3285_init(&rtc);

	/* UART i8251 (Terminal) @ level 2 */
	uart.base.mem_start = 0xA800;
	uart.base.mem_end = 0xAFFF;
	uart.base.enabled = true;
    uart.on_new_char = callback_on_new_char;
    uart.on_fetch_char = callback_on_fetch_char;
    uart.on_kbhit_char = callback_on_kbhit_char;
    uart.callback_userdata = (void*)terminal;
	pe_i8251_init(&uart);


	pe_set_devices_array(pe_list, sizeof(pe_list)/(sizeof(pe_list[0])));
}

void pe_end(void)
{
	fclose(rom1.fhandle);
	fclose(rom2.fhandle);
	fclose(rom3.fhandle);
	fclose(rom4.fhandle);
}


static bool _stop = false;

static SDL_sem *_got_input;
static uint8_t _input = 0;

int rmk_thread(void *data)
{
	uint32_t		cycle_count = 0;
	uint32_t		cycles_executed = 0;

    SDL_Terminal *terminal = (SDL_Terminal*)data;

	pe_start(terminal);

	cpu.pc = 0;
	cpu.io_read = cpu_in;
	cpu.io_write = cpu_out;
	cpu.mem_read = cpu_mem_read;
	cpu.mem_write = cpu_mem_write;

    while (!_stop)
    {
        if ((cycles_executed = rmk_run(&cpu)) == 0)
        {
            close_app = true;
            break;
        }

        cycle_count += cycles_executed;

        if (cycle_count % 32 == 0)
        {
            SDL_Delay(1);
        }
    }

    pe_end();
}


void key_press (SDL_Keysym *keysym)
{
    switch (keysym->sym) {
	case SDLK_ESCAPE:
		SDL_Quit ();
		exit (EXIT_SUCCESS);
	    break;
	default:
	    break;
	}
}
#define SDL_RMK_UART_EVENT (SDL_USEREVENT+1)

int callback_on_new_char(uint8_t new_char, void * userdata)
{
    SDL_Terminal *terminal = (SDL_Terminal*)userdata;
    if (!terminal)
    {
        return -1;
    }

    SDL_Event new_event;
    new_event.type = SDL_RMK_UART_EVENT;
    new_event.user.code = new_char;
    new_event.user.data1 = 0;

    switch (new_char)
    {
        case '\f':
        {
            //SDL_TerminalClear(terminal);
            new_event.user.data1 = (void*)1;
            break;
        }
        case '\r':
        {
            return 0;
            break;
        }
        default:
        {
            //SDL_TerminalPrint(terminal, "%c", new_char);
            break;
        }
    }

    SDL_PushEvent (&new_event);

    return 0;
}

int callback_on_fetch_char(uint8_t *new_char, void * userdata)
{
    if (SDL_SemValue(_got_input))
    {
        if (new_char)
        {
            *new_char = _input;
            SDL_SemWait(_got_input);
            return 0;
        }
    }

    return -1;
}

int callback_on_kbhit_char(bool *have_data, void * userdata)
{
    if (have_data)
    {
        *have_data = SDL_SemValue(_got_input) ? true : false;
        return 0;
    }

    return -1;
}

int main (int argc, char **argv)
{
    int return_status = EXIT_FAILURE;
    Uint32 last_tick = 0;
    const char font[] = "droid_sans_mono.ttf";
    int fps = 50;
    SDL_Surface *image;
    SDL_Thread *rmk_th;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
	    fprintf(stderr, "SDL2 Video initialization failed: %s\n", SDL_GetError());
        goto cleanup_sdl;
	}

    SDL_Window *window = SDL_CreateWindow("RMK8085 SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 860, 545, 0);
    if (!window)
    {
        fprintf(stderr, "Can't create main SDL2 window: %s\n", SDL_GetError());
        goto cleanup_sdl;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Can't create an SDL2 renderer: %s\n", SDL_GetError());
        goto cleanup_sdl_window;
    }

    SDL_Terminal *terminal = SDL_CreateTerminal();
    if (!terminal)
    {
        fprintf(stderr, "Can't create an SDL2 terminal: %s\n", SDL_GetError());
        goto cleanup_sdl_renderer;
    }

    if (SDL_TerminalSetFont(terminal, font, 12) < 0)
    {
        fprintf(stderr, "Selected font (%s) was not found or couldn't be opened!\n", font);
        goto cleanup_sdl_terminal;
    }

    SDL_TerminalSetSize(terminal, 80, 25);
    SDL_TerminalSetPosition(terminal, 0, 0);
    SDL_TerminalSetScaling(terminal, 1.5, 1.5);
    SDL_TerminalDisableKB(terminal);

    SDL_TerminalSetDefaultBackground(terminal, 0, 0, 0, 255);
    SDL_TerminalSetDefaultForeground(terminal, 255, 255, 255, 255);
    SDL_TerminalSetBorderColor (terminal, 255,255,255,255);
    SDL_TerminalReset(terminal);

    SDL_TerminalClear(terminal);
    SDL_TerminalPrint(terminal, "Terminal initialized\n");
    SDL_TerminalReset(terminal);

    _got_input = SDL_CreateSemaphore(0);

    rmk_th = SDL_CreateThread(rmk_thread, "rmk_thread", terminal);

    int done = 0;
    SDL_Event event;
    last_tick = SDL_GetTicks();

    while (!done) {
	    while (SDL_PollEvent(&event)) {
		    switch (event.type) {
			case SDL_KEYDOWN:
			    key_press (&event.key.keysym);
                if ((event.key.keysym.sym == SDLK_RETURN) || (event.key.keysym.sym == SDLK_KP_ENTER))
                {
                    if (SDL_SemValue(_got_input))
                    {
                        continue;
                    }

                    _input = '\r';
                    SDL_SemPost(_got_input);
                }
                else if (event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (SDL_SemValue(_got_input))
                    {
                        continue;
                    }

                    _input = 8;
                    SDL_SemPost(_got_input);
                }
			    break;
			case SDL_QUIT:
			    done = 1;
			    break;
            case SDL_TERMINALEVENT:
                printf ("Terminal event: %s\n", (char *) event.user.data2);
                break;
            case SDL_RMK_UART_EVENT:
            {
                if ((int)event.user.data1 == 1)
                {
                    SDL_TerminalClear(terminal);
                }
                else if ((int)event.user.data1 == 0)
                {
                    SDL_TerminalPrint(terminal, "%c", event.user.code);
                }
                break;
            }
            case SDL_TEXTINPUT:
            {
                if (SDL_SemValue(_got_input))
                {
                    continue;
                }

                _input = event.text.text[0];
                SDL_SemPost(_got_input);
                break;
            }
			default:
			    break;
			}
		}

        Uint32 wait = (Uint32)(1000.0f/fps);
        Uint32 new_tick  = SDL_GetTicks();
        if ((new_tick - last_tick) < wait)
        {
            SDL_Delay (wait - (new_tick-last_tick));
        }
        last_tick = SDL_GetTicks();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black
        SDL_RenderClear(renderer);
        SDL_TerminalBlit (terminal, renderer);

        SDL_RenderPresent(renderer);
	}

    _stop = true;
    SDL_WaitThread(rmk_th, NULL);
    rmk_th = NULL;

cleanup_sdl_terminal:
    SDL_DestroyTerminal(terminal);

cleanup_sdl_renderer:
    SDL_DestroyRenderer(renderer);

cleanup_sdl_window:
    SDL_DestroyWindow(window);

cleanup_sdl:
    SDL_Quit();

    exit(return_status);
	return 0;
}
