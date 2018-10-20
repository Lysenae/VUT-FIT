/* ************************************************************************** *
 * Autor:   Daniel Klimaj; xklima22@stud.fit.vutbr.cz                         *
 * Zmeny:   80%                                                               *
 * Revizia: 2015-12-18                                                        *
 * ************************************************************************** */

/*******************************************************************************
   main.c: LCD + keyboard demo
   Copyright (C) 2009 Brno University of Technology,
                      Faculty of Information Technology
   Author(s): Zdenek Vasicek <vasicek AT stud.fit.vutbr.cz>

   LICENSE TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. All advertising materials mentioning features or use of this software
      or firmware must display the following acknowledgement:

        This product includes software developed by the University of
        Technology, Faculty of Information Technology, Brno and its
        contributors.

   4. Neither the name of the Company nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

   This software or firmware is provided ``as is'', and any express or implied
   warranties, including, but not limited to, the implied warranties of
   merchantability and fitness for a particular purpose are disclaimed.
   In no event shall the company or contributors be liable for any
   direct, indirect, incidental, special, exemplary, or consequential
   damages (including, but not limited to, procurement of substitute
   goods or services; loss of use, data, or profits; or business
   interruption) however caused and on any theory of liability, whether
   in contract, strict liability, or tort (including negligence or
   otherwise) arising in any way out of the use of this software, even
   if advised of the possibility of such damage.

   $Id$


*******************************************************************************/

#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>
#include <stdlib.h>
#include <stdio.h>

#define BITS 16
#define FRACS 8
#define MAX 255
#define MAX_CNT 3

char last_ch; //naposledy precteny znak
char char_cnt;
unsigned int input, irslt, drslt;

/* Prototypy Vlastnych funkcii */
unsigned int ipowx(unsigned char n, char x);
unsigned int to_qn(unsigned char num, char qn);
void to_float(unsigned int num, unsigned char *intp, unsigned char *decp);
void set_bit(unsigned char *n, char bit);
void clr_bit(unsigned char *n, char bit);
void rshift_bit(unsigned char *n, char bit);
void new_bit(unsigned char *n, char bit);
unsigned char fisqrt(unsigned int n);
unsigned char cdcsqrt(unsigned int n);
void fast_int_sqrt(unsigned int n, unsigned char *irslt, unsigned char *drslt);
void cordic_sqrt(unsigned int n, unsigned char *irslt, unsigned char *drslt);

/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
void print_user_help(void)
{
}

/*******************************************************************************
 * Obsluha klavesnice
*******************************************************************************/
int keyboard_idle()
{
  char ch;
  char buf[10];

  ch = key_decode(read_word_keyboard_4x4());
  if(ch != last_ch)
  {
    last_ch = ch;
    if(ch != 0 && !(ch == '#' || ch == '*' || ch == 'C' || ch == 'D'))
    {
      if(char_cnt == 0)
      {
        LCD_clear();
      }

      if(ch == 'A' || ch == 'B')
      {
        /* V zavislosti od poctu zadanych cifier sa vydeli vstupne cislo */
        if(char_cnt < MAX_CNT)
        {
          input = input / ipowx(10, MAX_CNT - char_cnt);
        }

        if(input <= 255)
        {
          if(ch == 'A') /* A - Vypocet pomocou Fast Int Sqrt */
          {
            fast_int_sqrt(input, &irslt, &drslt);
            term_send_crlf();
            term_send_str("FastIntSqrt(");
            term_send_num(input);
            term_send_str("): ");
            sprintf(buf, "%d.%d", irslt, drslt);
            term_send_str(buf);
            LCD_write_string(buf);
          }
          else if(ch == 'B') /* B - Vypocet pomocou CORDIC */
          {
            cordic_sqrt(input, &irslt, &drslt);
            term_send_crlf();
            term_send_str("CORDIC(");
            term_send_num(input);
            term_send_str("): ");
            sprintf(buf, "%d.%d", irslt, drslt);
            term_send_str(buf);
            LCD_write_string(buf);
          }
        }
        else
        {
          term_send_crlf();
          term_send_str("Chyba rozsahu: cislo musi byt z intervalu <0;255>");
          LCD_write_string("ERROR");
        }

        input = 0;
        ch = 0;
        char_cnt = 0;
      }

      if(ch != 0)
      {
        if(char_cnt == MAX_CNT)
        {
          LCD_clear();
          char_cnt = 0;
          input = 0;
        }

        /* Vypocet vstupneho cisla, prevod char na uint */
        input = input + ((ch - 48) * ipowx(10, MAX_CNT - char_cnt - 1));
        LCD_append_char(ch);
        char_cnt++;
      }
    }
  }
  return 0;
}

/*******************************************************************************
 * Dekodovani a vykonani uzivatelskych prikazu
*******************************************************************************/
unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
  return CMD_UNKNOWN;
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
void fpga_initialized()
{
  LCD_init();
  LCD_clear();
}

/*******************************************************************************
 * Hlavni funkce
*******************************************************************************/
int main(void)
{
  char_cnt = 0;
  last_ch = 0;
  input = 0;

  initialize_hardware();
  keyboard_init();

  while (1)
  {
    delay_ms(10);
    keyboard_idle();                   // obsluha klavesnice
    terminal_idle();                   // obsluha terminalu
  }
}

/* *** VLASTNE FUNKCIE *** */

/**
 * Vypocet x-tej mocniny cisla n.
 * @param n Mocnenec
 * @param x Exponent
 * @return mocninu x-tu mocninu cisla n
 **/
unsigned int ipowx(unsigned char n, char x)
{
  if(x == 0)
  {
    return 1;
  }

  unsigned int rslt = n;
  unsigned int prev = n;
  char i;
  for(i=1; i<x; i++)
  {
    rslt = rslt * n;
    if(rslt < prev)
    {
      term_send_crlf();
      term_send_str("Overflow");
      term_send_crlf();
      term_send_str("> ");
      return 0;
    }
    prev = rslt;
  }

  return rslt;
}

/**
 * Prevod cisla do Q formatu.
 * @param num Prevadzane cislo
 * @param qn Pocet desatinnych cisel
 * @return cislo v Q formate.
 **/
unsigned int to_qn(unsigned char num, char qn)
{
  return num * ipowx(2, qn);
}

/**
 * Prevod celeho cisla na cislo s plavajucou desatinnou ciarkou.
 * @param num Prevadzane cislo
 * @param intp Ciselna premenna, do ktorej sa ulozi cela cast
 * @param decp Ciselna premenna, do ktorej sa ulozi desatinna cast
 **/
void to_float(unsigned int num, unsigned char *intp, unsigned char *decp)
{
  *intp = num / ipowx(2, FRACS/2);
  *decp = num % ipowx(2, FRACS/2) * 100 / (ipowx(2, FRACS/2)-1);
}

/**
 * Nastavi bit na 1.
 * @param n Cislo, v ktorom sa nastavuje bit
 * @param bit Index bitu
 **/
void set_bit(unsigned char *n, char bit)
{
  if(bit >= 0)
  {
    *n = (*n | 1 << bit);
  }
}

/**
 * Nastavi bit na 0.
 * @param n Cislo, v ktorom sa nastavuje bit
 * @param bit Index bitu
 **/
void clr_bit(unsigned char *n, char bit)
{
  if(bit >= 0)
  {
    *n = (*n & ~(1 << bit));
  }
}

/**
 * Posunie bit doprava.
 * @param n Cislo, v ktorom sa posuva bit
 * @param bit Index bitu
 **/
void rshift_bit(unsigned char *n, char bit)
{
  clr_bit(n, bit);
  set_bit(n, bit-1);
}

/**
 * @see set_bit()
 **/
void new_bit(unsigned char *n, char bit)
{
  set_bit(n, bit);
}

/**
 * Vypocet odmocniny metodou Fast Integer Square Root.
 * @param n Odmocnovane cislo
 * @return odmocninu cisla n
 **/
unsigned char fisqrt(unsigned int n)
{
  unsigned char rslt = 0x80;
  unsigned int temp;
  char i;

  for(i=BITS/2-1; i>=0; i--)
  {
    temp = ipowx(rslt, 2);
    if(temp > n)
    {
      rshift_bit(&rslt, i);
    }
    else if(temp < n)
    {
      new_bit(&rslt, i-1);
    }
    else
    {
      break;
    }
  }

  return rslt;
}

/**
 * Vypocet odmocniny metodou CORDIC.
 * @param n Odmocnovane cislo
 * @return odmocninu cisla n
 **/
unsigned char cdcsqrt(unsigned int n)
{
  unsigned int base, i, rslt;
  base = 0x80;
  rslt = 0;

  for(i = 1; i<=BITS; i++)
  {
    rslt = rslt + base;
    if((rslt * rslt) > n)
    {
      rslt = rslt - base;
    }

    base = base >> 1;
  }

  return rslt;
}

/**
 * Rozhranie pre vypocet odmocniny metodou Fast Integer Square Root.
 * @param n Odmocnovane cislo
 * @param irslt Cislo, do ktoreho sa ulozi cela cast odmocniny
 * @param drslt Cislo, do ktoreho sa ulozi desatinna cast odmocniny
 **/
void fast_int_sqrt(unsigned int n, unsigned char *irslt, unsigned char *drslt)
{
  to_float((fisqrt(to_qn(n, FRACS))), irslt, drslt);
}

/**
 * Rozhranie pre vypocet odmocniny metodou CORDIC.
 * @param n Odmocnovane cislo
 * @param irslt Cislo, do ktoreho sa ulozi cela cast odmocniny
 * @param drslt Cislo, do ktoreho sa ulozi desatinna cast odmocniny
 **/
void cordic_sqrt(unsigned int n, unsigned char *irslt, unsigned char *drslt)
{
  to_float((cdcsqrt(to_qn(n, FRACS))), irslt, drslt);
}
