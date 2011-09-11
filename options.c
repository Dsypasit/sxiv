/* sxiv: options.c
 * Copyright (c) 2011 Bert Muennich <muennich at informatik.hu-berlin.de>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#define _POSIX_C_SOURCE 200112L
#define _IMAGE_CONFIG

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "options.h"
#include "util.h"
#include "config.h"

options_t _options;
const options_t *options = (const options_t*) &_options;

void print_usage() {
	printf("usage: sxiv [-cdFfhpqrstvZ] [-g GEOMETRY] [-n NUM] "
	       "[-z ZOOM] FILES...\n");
}

void print_version() {
	printf("sxiv %s - Simple X Image Viewer\n", VERSION);
	printf("Additional features included (+) or not (-): %s, %s\n",
#ifdef EXIF_SUPPORT
	       "+exif",
#else
	       "-exif",
#endif
#ifdef GIF_SUPPORT
	       "+gif"
#else
	       "-gif"
#endif
	      );
}

void parse_options(int argc, char **argv) {
	int opt, t;

	_options.recursive = false;
	_options.startnum = 0;

	_options.scalemode = SCALE_MODE;
	_options.zoom = 1.0;
	_options.aa = true;

	_options.fixed_win = false;
	_options.fullscreen = false;
	_options.geometry = NULL;

	_options.quiet = false;
	_options.thumb_mode = false;
	_options.clean_cache = false;

	while ((opt = getopt(argc, argv, "cdFfg:hn:pqrstvZz:")) != -1) {
		switch (opt) {
			case '?':
				print_usage();
				exit(1);
			case 'c':
				_options.clean_cache = true;
				break;
			case 'd':
				_options.scalemode = SCALE_DOWN;
				break;
			case 'F':
				_options.fixed_win = true;
				break;
			case 'f':
				_options.fullscreen = true;
				break;
			case 'g':
				_options.geometry = optarg;
				break;
			case 'h':
				print_usage();
				exit(0);
			case 'n':
				if (!sscanf(optarg, "%d", &t) || t < 1) {
					fprintf(stderr, "sxiv: invalid argument for option -n: %s\n",
					        optarg);
					exit(1);
				} else {
					_options.startnum = t - 1;
				}
				break;
			case 'p':
				_options.aa = false;
				break;
			case 'q':
				_options.quiet = true;
				break;
			case 'r':
				_options.recursive = true;
				break;
			case 's':
				_options.scalemode = SCALE_FIT;
				break;
			case 't':
				_options.thumb_mode = true;
				break;
			case 'v':
				print_version();
				exit(0);
			case 'Z':
				_options.scalemode = SCALE_ZOOM;
				_options.zoom = 1.0;
				break;
			case 'z':
				_options.scalemode = SCALE_ZOOM;
				if (!sscanf(optarg, "%d", &t) || t <= 0) {
					fprintf(stderr, "sxiv: invalid argument for option -z: %s\n",
					        optarg);
					exit(1);
				}
				_options.zoom = (float) t / 100.0;
				break;
		}
	}

	_options.filenames = argv + optind;
	_options.filecnt = argc - optind;
	_options.from_stdin = _options.filecnt == 1 &&
	                      !strcmp(_options.filenames[0], "-");
}
