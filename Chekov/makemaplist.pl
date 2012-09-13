my $line;
while ($line = <STDIN>)
{
	$line =~ s/(\d+)\t(\d+)\t(\d+)\t(\d+)\t(\d+)\t(\d+)\t(\d+)\t(\d+)\t(\d\d)(\d\d)(\d\d)/$5 $2 $1 $4 $9\/$9$10\/$9$10$11.tif/;
	print $line;
}
