--TEST--
Phar: opendir test, recurse into
--SKIPIF--
<?php if (!extension_loaded("phar")) print "skip"; ?>
--FILE--
<?php
$file = "<?php
Phar::mapPhar('hio');
__HALT_COMPILER(); ?>";

$files = array();
$files['a'] = 'a';
$files['b/a'] = 'b';
$files['b/c/d'] = 'c';
$files['bad/c'] = 'd';
$manifest = '';
foreach($files as $name => $cont) {
	$len = strlen($cont);
	$manifest .= pack('V', strlen($name)) . $name . pack('VVVVC', $len, time(), $len, crc32($cont), 0x00);
}
$alias = 'hio';
$manifest = pack('VnV', count($files), 0x0800, strlen($alias)) . $alias . $manifest;
$file .= pack('V', strlen($manifest)) . $manifest;
foreach($files as $cont)
{
	$file .= $cont;
}

file_put_contents(dirname(__FILE__) . '/' . basename(__FILE__, '.php') . '.phar.php', $file);
include dirname(__FILE__) . '/' . basename(__FILE__, '.php') . '.phar.php';

function dump($phar, $base)
{
	var_dump($phar . $base);
	$dir = opendir($phar . $base);
	if ($base == '/')
	{
		$base = '';
	}
	while (false !== ($entry = readdir($dir))) {
		$entry = $base . '/' . $entry;
		var_dump($entry);
		var_dump(is_dir($phar . $entry));
		if (is_dir($phar . $entry))
		{
			dump($phar, $entry);
		}
	}
}

dump('phar://hio', '/');

?>
--CLEAN--
<?php unlink(dirname(__FILE__) . '/' . basename(__FILE__, '.clean.php') . '.phar.php'); ?>
--EXPECT--
string(11) "phar://hio/"
string(2) "/a"
bool(false)
string(2) "/b"
bool(true)
string(12) "phar://hio/b"
string(4) "/b/a"
bool(false)
string(4) "/b/c"
bool(true)
string(14) "phar://hio/b/c"
string(6) "/b/c/d"
bool(false)
string(4) "/bad"
bool(true)
string(14) "phar://hio/bad"
string(6) "/bad/c"
bool(false)
