# php-aho

php-aho is a small wrapper for the [multifast](http://multifast.sourceforge.net/)'s [Aho–Corasick algorithm](https://en.wikipedia.org/wiki/Aho%E2%80%93Corasick_algorithm).

## Installation
```
phpize
./configure
make
sudo make install
```

## API overview

define `array aho_match(string $subject)` method.

## Usage
Enable `php-aho` and set dict path in the ini file:
```ini
[aho]
extension=aho.so
aho.dict=/tmp/foo.dict
```

An example dict is like this:
```
apple`fruit
china`nation
smile`emotion
```
Use the __`__ as the separator.

And then:
```
Psy Shell v0.7.2 (PHP 7.0.8 — cli) by Justin Hileman
>>> aho_match ('apple from china make me smile')
=> [
     [
       0,        # keyword offset
       "apple",  # keyword
       "fruit",  # keyword type
     ],
     [
       11,
       "china",
       "nation",
     ],
     [
       25,
       "smile",
       "emotion",
     ],
   ]
```

Enjoy yourself :).
