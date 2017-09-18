/*
 * Copyright (C) 2016  Nexell Co., Ltd.
 * Author: Sangjong, Han <hans@nexell.co.kr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(Vectors)

INTERNAL_SRAM_START = 0xFFFF0000;
INTERNAL_SRAM_SIZE = 32 * 1024;

__stack_percpu_size	= 1 * 1024;
__stack_main_additional = 1*1024;

MEMORY {
	iram (rwx) : ORIGIN = INTERNAL_SRAM_START, LENGTH = INTERNAL_SRAM_SIZE
}

SECTIONS
{
	.text : {
		. = ALIGN(4);
		__image_start__ = .;
		KEEP(./src/header.o(.text))
		*(.text*)
	} > iram
	
	.rodata : {
		. = ALIGN(4);
		*(.rodata)
	} > iram
	
	.data : {
		. = ALIGN(4);
		*(.data*)
	} > iram
	
	.bss_start (NOLOAD) : {
		. = ALIGN(4);
		__bss_start__ = .;
		*(.__bss_start)
	} > iram

	.bss : {
		. = ALIGN(4);
		*(.COMMENT*)
		*(.bss*)
	} > iram

	.bss_end (NOLOAD) : {
		. = ALIGN(4);
		*(.__bss_end)
		__bss_end__ = .;
	} > iram
	
	.comment : {
		*(.comment*)
	} > iram

	
	tlb : {
		. = ALIGN(0x4000);
		__tlb_base = .;
		. += 0x1000;
	} >iram
	
	.stack (NOLOAD) :
	{
	 	. = ALIGN(8);
	 	__stack_start = . ;
		. += (__stack_percpu_size * 8);
		. += __stack_main_additional;
		__stack_end = . ;
	} > iram
	
	__image__end__ = .;
	end = .;
}

__image_size__ = (__image__end__ - __image_start__ + 0x10 - 1 ) & 0xFFFFFFF0;
ASSERT(__image_size__ < INTERNAL_SRAM_SIZE, "error: image is too big!!!");


