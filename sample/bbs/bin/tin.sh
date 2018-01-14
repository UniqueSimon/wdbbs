#!/bin/sh
#
# Last updated : by netty, Apr.11,1995
#
PATH=/sbin:/bin:/usr/sbin:/usr/bin:/usr/etc:/usr/local/bin
clear
stty pass8
stty -parenb cs8 -istrip
username=$1
nickname=$2
echo "<<< �w��ϥΤ����j�Ǹ�u�t�Ҥ� news reader >>>"
echo ""
echo "[�����m�{]  �Y�z�O�Ĥ@���ϥΥ��\��"
echo "            �z�b�i�J����  �Ҧ��Q�װϬҬ����q"
echo "            ��ĳ�ϥ� S ���O, �N�h�ӰQ�װϭq�\�W��"
echo ""
echo "[���ܻ���]  �Y�z�b�i�J����  �T���H�^��X�{"
echo "            ��ĳ�ϥ� D ���O, �H������������ܼҦ�"
echo ""
HOME=home/$username
export HOME
if [ -f $HOME/signatures ] ; then
	/bin/split -6 $HOME/signatures  $HOME/.Sig
        /bin/mv $HOME/.Sigaa $HOME/.Siga1
if [ -f $HOME/.Sigab ] ; then
    /bin/mv $HOME/.Sigab $HOME/.Siga2
if [ -f $HOME/.Sigac ] ; then	
    /bin/mv $HOME/.Sigac $HOME/.Siga3
fi
if [ -f $HOME/.Sigad ] ; then	
    /bin/mv $HOME/.Sigad $HOME/.Siga4
fi
if [ -f $HOME/.Sigae ] ; then	
    /bin/mv $HOME/.Sigae $HOME/.Siga5
fi
if [ -f $HOME/.Sigaf ] ; then	
    /bin/mv $HOME/.Sigaf $HOME/.Siga6
fi
if [ -f $HOME/.Sigag ] ; then	
    /bin/mv $HOME/.Sigag $HOME/.Siga7
fi
if [ -f $HOME/.Sigah ] ; then	
    /bin/mv $HOME/.Sigah $HOME/.Siga8
fi
if [ -f $HOME/.Sigai ] ; then	
    /bin/mv $HOME/.Sigai $HOME/.Siga9
fi
	echo -n "�п��ñ�W��: 1-9  < (1) "
	read me8
	echo ""
	me9=.Siga$me8
	if [ $me9 != .Siga ] ; then
	    if [ -f $HOME/$me9 ] ; then	
	        /bin/mv $HOME/$me9 $HOME/.signature
#	        echo "            �z��β� $me8 ��ñ�W��" 
#	        echo ""
	    else
	        /bin/mv $HOME/.Siga1 $HOME/.signature    
	        echo "  �u�X�X!!  �z�õL��ñ�W��, netty ���z��� 1 
��ñ�W��" 
	        echo ""
	    fi
	else
	    /bin/mv $HOME/.Siga1 $HOME/.signature    
#	    echo "            �z��β� 1 ��ñ�W��" 
#  	    echo ""
	fi
else
#	echo "[���a�߮�]  �n��!!  �z���@��ñ�W��"
#	echo ""
	/bin/mv $HOME/.Siga1 $HOME/.signature    
fi
else
	echo "[��������]  �z�|���s��ۤv��ñ�W��"
	echo ""
fi
if [ -f $HOME/.newsrc ] ; then
	/bin/sort -o $HOME/.newsrc $HOME/.newsrc
fi
EDITOR=bin/celvis
echo -n "�п�νs�边 (1)bbs (2)joe (3)vi < (1) "
read me4
echo ""
me5=a$me4
if [ $me5 != a3 ] ; then
	if [ $me5 != a2 ] ; then
	    EDITOR=bin/ve
	    if [ $me5 != a1 ] ; then
	        if [ $me5 != a ] ; then
	            echo "  �u�X�X!!  �z����ܦ��~, netty ���z�� bbs 
editor" 
	            echo ""
	        fi
            fi		
#	    echo "            �z���s�边�� bbs editor "
	    echo ""
	else
		EDITOR=bin/joe
#		echo "            �z���s�边�� joe "
		echo ""
	fi		
else
#	echo "            �z���s�边�� vi"
	echo ""
fi
export EDITOR
#echo -n "�Ы� <Enter> �� <Return> �H�i�J���\�� < "
#read me2
#clear
lcctype=iso_8859_1;
LC_CTYPE=$lcctype; export LC_CTYPE
nntpserver=news.cs.ccu.edu.tw
NNTPSERVER=$nntpserver; export NNTPSERVER
USER=${username}.bbs; export USER
NAME=${nickname}; export NAME
ORGANIZATION="������u�|�ʦ~�ӲĤ@��"; export ORGANIZATION
#
trap quit 0 1 2 3 15
#
tin
#
quit () {
#	kill `ps -aux | grep 'tail -f' | grep -v grep |awk '{print $1}'` 
> /dev/null 2>&1 
	echo ""
	echo ""
	echo "[�{�O�ب�]  ���±z�����{"
	echo ""
	echo "[�өv�n��]  �Y������i���B, �Цbsysop�O���d�U�z����ĳ"
	echo "            �z�{�p�@����l, �ڭ̮ɮɻݭn !!"
	echo ""
        echo -n "�Ы� <Enter> �� <Return> �H�������\�� < "
read me
}
