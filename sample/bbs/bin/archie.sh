echo
echo "�j�M��, �еy��....."
echo "�i�j�a�J��a�j ARCHIE Service." `date` > /home/bbs/tmp/archie.$USER
echo "" >> /home/bbs/tmp/archie.$USER
echo "Search for: $ARCHIESTRING " >> /home/bbs/tmp/archie.$USER
exec /usr/local/bin/archie -c $ARCHIESTRING >> /home/bbs/tmp/archie.$USER

