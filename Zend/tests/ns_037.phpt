--TEST--
037: Name ambiguity (namespace name or namespace's class name)
--FILE--
<?php
namespace X;
import X as Y;
class X {
	const C = "const ok\n";
	function __construct() {
		echo "class ok\n";
	}
	static function bar() {
		echo "method ok\n";
	}
}
new X();
new X::X();
new Y::X();
new ::X::X();
X::bar();
X::X::bar();
Y::X::bar();
::X::X::bar();
echo X::C;
echo X::X::C;
echo Y::X::C;
echo ::X::X::C;
--EXPECT--
class ok
class ok
class ok
class ok
method ok
method ok
method ok
method ok
const ok
const ok
const ok
const ok
