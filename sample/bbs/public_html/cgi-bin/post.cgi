#! /usr/bin/perl

# Get Parameters
if ($ENV{'REQUEST_METHOD'} eq "POST") {
   read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
} elsif ($ENV{'REQUEST_METHOD'} eq "GET") {
        $buffer=$ENV{'QUERY_STRING'};
}

# Make Pair of Name and Value
@pairs = split(/&/, $buffer);
foreach $pair (@pairs)
{
    ($name, $value) = split(/=/, $pair);

    $value =~ tr/+/ /;
    $value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
    $INPUT{$name} = $value;
}

print "Content-type: text/html\n\n";

if ($INPUT{'id'} eq "") 
  { print "�п�J�z�b WD-BBS �W�� ID\n"; exit;}
elsif ($INPUT{'passwd'} eq "") 
  { print "�п�J�z�b WD-BBS �W���K�X\n"; exit;}
elsif ($INPUT{'board'} eq "") 
  { print "�п�J�z�n�K���ݪO\n"; exit;}
elsif ($INPUT{'subject'} eq "") 
  { print "�п�J�峹�����D\n"; exit;}
elsif ($INPUT{'article'} eq "") 
  { print "����K�ťժ��峹��!\n"; exit;}
else {
        open(TMP, "> /home/bbs/tmp/mailpost_$INPUT{'id'}");
        print TMP "#name: $INPUT{'id'}\n";
        print TMP "#password: $INPUT{'passwd'}\n";
        print TMP "#board: $INPUT{'board'}\n";
        print TMP "#title: $INPUT{'subject'}\n\n";
        print TMP "$INPUT{'article'}\n\n";
        print TMP "--\n[1;36m��Post by [37m$INPUT{'id'} [36mfrom [33m$ENV{'REMOTE_ADDR'}[m\n";
        close(TMP);
	system("/usr/bin/mail bbs < /home/bbs/tmp/mailpost_$INPUT{'id'}");
	system("/bin/rm /home/bbs/tmp/mailpost_$INPUT{'id'}");
        print "<body bgcolor=000000 text=ffff70>";   
        print "�Ӧ� $ENV{'REMOTE_ADDR'} �� $INPUT{id} �z�n...<br>";
        print "�z���峹�w�g�e�X ... �p�G�b���αK�X���~�|�L�k�K�W !<br>";
}
