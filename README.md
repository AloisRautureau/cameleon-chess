<p align="center">
  <img src="https://github.com/AloisRautureau/chameleon-chess/blob/master/camaelon-icon.png?raw=true" alt='The Chameleon logo'/>
</p>


# chameleon-chess

chameleon-chess is the **chess engine** which will be powering up BAUB-Chess in the near future. The goal is to make it a fully functionnal, decent, and UCI compatible engine, optimized enough to run correctly on a Raspberry Pi.<br/>
It uses a combination of a 0x88 board representation, speed up by keeping a piece-list to avoid scanning it in its entirety.

## TODO

[x] Board Representation<br/>
[x] Evaluation function<br/>
[x] Search function<br/>
[x] UCI compatibility (partial at the time)<br/>
[ ] Deep optimization<br/>
[ ] Opening book<br/>

## How do I use chameleon ?

**!!! DISCLAIMER !!!**
**The program as it is now is given as a proof of concept, it is still really buggy and unoptimised and shouldn't be considered as a finished product**

You can acquire the engine by **compiling the sources** at this point. Releases will be uploaded when a lot more bugs have been ironed out.<br>
The program uses an UCI implementation, and while it can be used directly in a terminal, it is best used in combination with a UCI-compatible GUI (e.g Arena, Lucas, ChessBase...)<br>

Here is a list of commands which are useful when using the program as a CLI<br>
Note that it can also be found on http://wbec-ridderkerk.nl/html/UCIProtocol.html<br>
<ul>
<li> ucinewgame: tells the program the position we cant to search is from a different game</li>
<li> position [< fen > | startpos] moves <x> <y>... : sets the position to either the given fen string or the start position, then plays the given moves</li>
<li> show : prints the current position int the terminal </li>
<li> go : fires up a search! Here is a list of arguments for that particular command :
	<ul>
	<li> searchmoves ... : gives a list of moves to search </li>
	<li> wtime <x> : sets white side's time left </li>
	<li> btime <x> : sets black side's time left </li>
	<li> winc <x> : sets the time increment for white </li>
	<li> binc <x> : sets the time increment for black </li>
	<li> depth <x> : gives a max depth to the search </li>
	<li> nodes <x> : sets a max number of nodes the engine can search </li>
	<li> movetime <x> : sets the max time allocated to the search in ms </li>
	<li> infinite : a boolean that, when set, let's the search continue on forever </li>
	</ul>
<li> quit : stops the program </li>
</ul>

