--TEST--
count_chars() function
--POST--
--GET--
--FILE--
<?php
$s = "het leven is net erwtensoep - je kunt er geen touw aan vastknopen";
for($i=0; $i<3; $i++) {
	echo implode(count_chars($s, $i))."\n";
}
echo $a = count_chars($s, 3), "\n";
echo (int) strlen(count_chars($s, 4)) == 256-strlen($a),"\n";

?>
--EXPECT--
000000000000000000000000000000001200000000000010000000000000000000000000000000000000000000000000003000120111121083202362220000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
121312111121832236222
000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 -aeghijklnoprstuvw
1
