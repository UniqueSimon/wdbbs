#!/bin/sh
#
# Last updated date: Apr.03,1995
#
set LC_CTYPE=iso_8859_1
#biff n
TERM=vt100
export TERM
export LC_CTYPE
LOCALDOMAIN=.
export LOCALDOMAIN
stty pass8 -istrip
echo "  "
echo "�w����{ �i[1;33;44m���P�ЮJ�����[m�j �ϥ� [1mGopher[m"
echo "�O�o�^�ӧr!!"
echo "�Ы� <Enter> �� <Return> �H�i�J���\�� < "
read me1
exec /usr/local/bin/gopher -s cc.shu.edu.tw

