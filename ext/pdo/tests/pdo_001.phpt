--TEST--
PDO Common: PDO_FETCH_ASSOC
--SKIPIF--
<?php # vim:ft=php
if (!extension_loaded('pdo')) die('skip');
$dir = getenv('REDIR_TEST_DIR');
if (false == $dir) die('skip no driver');
require_once $dir . 'pdo_test.inc';
PDOTest::skip();
?>
--FILE--
<?php
require getenv('REDIR_TEST_DIR') . 'pdo_test.inc';
$db = PDOTest::factory();

$db->exec('CREATE TABLE test(id int NOT NULL PRIMARY KEY, val VARCHAR(10))');
$db->exec("INSERT INTO test VALUES(1, 'A')");
$db->exec("INSERT INTO test VALUES(2, 'B')");
$db->exec("INSERT INTO test VALUES(3, 'C')");

$stmt = $db->prepare('SELECT * from test');
$stmt->execute();

var_dump($stmt->fetchAll(PDO_FETCH_ASSOC));
?>
--EXPECT--
array(3) {
  [0]=>
  array(2) {
    ["id"]=>
    string(1) "1"
    ["val"]=>
    string(1) "A"
  }
  [1]=>
  array(2) {
    ["id"]=>
    string(1) "2"
    ["val"]=>
    string(1) "B"
  }
  [2]=>
  array(2) {
    ["id"]=>
    string(1) "3"
    ["val"]=>
    string(1) "C"
  }
}
--UEXPECT--
array(3) {
  [0]=>
  array(2) {
    [u"id"]=>
    unicode(1) "1"
    [u"val"]=>
    unicode(1) "A"
  }
  [1]=>
  array(2) {
    [u"id"]=>
    unicode(1) "2"
    [u"val"]=>
    unicode(1) "B"
  }
  [2]=>
  array(2) {
    [u"id"]=>
    unicode(1) "3"
    [u"val"]=>
    unicode(1) "C"
  }
}
