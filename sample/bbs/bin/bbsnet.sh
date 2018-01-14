
#!/bin/sh
#
#       define the standout() and standend() escape sequences...
#
SO="[7m"
SE="[0m"
#biff n
stty pass8 -istrip
echo "*[2J*[1;1H"
echo "�w����{ �i^[[1;33;44m�����~�{^[[m�j �ϥ� ^[[1mGopher^[[m"
echo "�O�o��p�ɤ��^�ӧr!!"
echo "�Ы� <Enter> �� <Return> �H�i�J���\�� < "
read me1

while true
do

clear

echo "  ${SO}  �ꤺ�U�j�� BBS ��  ${SE}  

     [1] �x�j�q��                        [11] �H��R 
     [2] ���s�j��                        [12] Tiger II 
     [3] ��j��u                        [13] �P�z��
     [4] �v�j�p��                        [14] ���s�n����
     [5] ����j��                        [15] �H�u���z����� X
     [6] ��j��� X                      [16] �ǩh�Q����
     [7] �M�j�ƾ� *                      [17] �����s�߯�
     [8] �F���j��                        [18] �ʷP 
     [9] ������u *                      [19] ���R�k��
     [10]���\�j�� * 			 [20] hinet   
                                         [21] ��᷽               
		 
        
  ${SO} �䥦�����A�� ${SE}

   [30] ��j��ugopher         [35] ��j MUD (port 4040)
   [31] ���s�j��gopher         [36] 
   [32] �����j��gopher         [37] �x�j�Ϯ��]��T�t��
   [33] ��q�j��hytelnet       [38] �����H��
   [34] �����j��archie         [39] �x�j MUD
                               [40] �x�j gopher"
echo -n "Enter choice [0]: " \

        if read CHOICE
          then
            clear
            echo -n "$USER `date`" 
            case "${CHOICE}"
              in
                '')
                  echo "" 
                  break
                  ;;
                0)
                  echo ""
                  break
                  ;;
                1)
                  echo "�x�j�q�� bbs" 
                   telnet 140.112.19.32
                   ;;
                2)
                  echo "���s  bbs" 
                   telnet 140.117.11.2
                   ;;
                3)
                  echo "��j��u bbs" 
                   telnet 140.113.17.154
                   ;;
                4)
                  echo "�v�j�p�� bbs"
                   telnet 140.122.65.19
                   ;;
                5)
                  echo "����j�� bbs"
                   telnet 140.135.18.12
                   ;;
                6)
                  echo "��j��� bbs" 
                   telnet 140.113.23.3 
                   ;;
                7)
                  echo "�M�j�ƾ�  bbs"
                   telnet 140.114.45.12
                   ;;
                8)
                  echo "�F���j�� bbs" 
                   telnet 140.128.99.114
                  ;;
		9)
		  echo "������u bbs" 
		  telnet 140.123.101.78 
		  ;;
                10)
                  echo "���\�j�� bbs" 
                   telnet 140.116.2.12
                  ;;
                11)
                  echo "�H��R" 
                   telnet 140.113.11.234
                  ;;
                12)
                  echo "Tiger II bbs" 
                  telnet 140.113.122.208
                  ;;
		13)
		  echo "�ּ֤Ѱ� bbs" 
		  telnet 140.115.249.100
		  ;;
		14)
		  echo "���s�n���� bbs"
		  telnet 140.117.11.4
		  ;;
		15)
		  echo "�H�u���z����� bbs"
		  telnet 140.114.78.31
		  ;;
                16)
                  echo "�ǩh�Q���� bbs"
                  telnet 140.113.30.91
                  ;;
                17)
                  echo "�����s�߯� bbs"
                  telnet 140.115.83.240
                  ;;
                18)
                  echo "�ʷP bbs"
                  telnet 140.113.122.157
                  ;;
                19)
                  echo "���R�k�� bbs"
                  telnet 140.113.189.34
                  ;;
                20)
		  echo "hinet"
		  telnet serv.hinet.net
	          ;;
                21)  
                   exho "��᷽"
		   telnet 140.115.234.5
		   ;;
		E)
                  echo "Eagle's Nest" 
                  telnet 131.95.127.2
                  ;;
                e)
                  echo "Eagle's Nest"
                  telnet 131.95.127.2
                  ;;
                B)
                  echo "Badboy" 
                  telnet 192.136.108.18
                  ;;
                b)
                  echo "Badboy" 
                  telnet 192.136.108.18
                  ;;
                A)
                  echo "Auggie" 
                  telnet 141.224.128.4
                  ;;
                a)
                  echo "Auggie" 
                  telnet 141.224.128.4
                  ;;
                U)
                  echo "UNINET" 
                  telnet 131.178.6.132
                  ;;
                u)
                  echo "UNINET" 
                  telnet 131.178.6.132
                  ;;
                Q)
                  echo "Quartz" 
                  telnet 128.6.60.6
                  ;;
                q)
                  echo "Quartz" 
                  telnet 128.6.60.6
                  ;;
                I)
                  echo "ISCA" 
                  telnet 128.255.40.203
                  ;;
                i)
                  echo "ISCA" 
                  telnet 128.255.40.203
                  ;;
                S)
                  echo "Sunset" 
                  telnet 128.196.230.7
                  ;;
                s)
                  echo "Sunset" 
                  telnet 128.196.230.7
                  ;;
                 30)
                  echo "��jgopher" 
                  telnet gopher.csie.nctu.edu.tw 4270
                  ;;
                 31)
                  echo "���sgopher" 
                  echo "Login as 'gopher'"
                  sleep 1
                  telnet gopher.nsysu.edu.tw
                  ;;
                 32)
                  echo "����gopher" 
                  echo "Login as 'gopher'"
                  sleep 1
                  telnet gopher.ccu.edu.tw
                  ;;
                 33)
                  echo "��jhytelnet" 
                  echo "Login as 'hytelnet'" 
                  sleep 1
                  telnet nctuccca.edu.tw
                  ;;
                 34)
                  echo "����archie" 
                  echo "Login as 'twarchie' or 'archie'"
                  sleep 1
                  telnet archie.ncu.edu.tw
                  ;;
                35)
                  echo "��j Merc DikuMud 2.0" 
                  telnet ccsun44.csie.nctu.edu.tw 4040
                  ;;
#                36)
#                  echo "��j The Reamls of Wind II" 
#                  telnet ccsun44.csie.nctu.edu.tw 1234
#                  ;;
                37)
                  echo "�x�j�Ϯ��]" 
                  echo "�H'tulips' login �i�i�J����d�ߥD�e��"
                  echo "or login as 'library' for English users"
                  echo ""
                  telnet 140.112.192.1
                  ;;
                38)
                  echo "�����H��" 
                  telnet 128.103.28.15 5555
                  ;;
                39)
                  echo "�x�j MUD" 
                  telnet 140.112.2.33 3000
                  ;;
		40)
		  echo "�x�jgopher" 
		  echo "Login as 'gopher'"
		  echo ""
		  telnet 140.112.8.19
                  ;;
                *)
                  echo "" 
                  echo ""
                  echo "${SO}${CHOICE} is an invalid option. Select again.${SE}" 
                  sleep 1
                  ;;
            esac
          else
            exit
        fi
done

clear
#biff y


