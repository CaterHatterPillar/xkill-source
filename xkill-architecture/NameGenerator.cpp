#include "NameGenerator.h"

#include <xkill-utilities\Math.h> //Math::randomInt

void NameGenerator::resetNames()
{
	names.clear();

	//
	// Add names
	//

	addName("Sprak");
	addName("Algol");
	addName("Haskell");
	addName("Fortran");
	addName("Pascal");
	addName("Erlang");
	addName("Lisp");
	addName("Occam");
	addName("Brainfuck");
	addName("Formac");
	addName("Quiktran");
	addName("Cowsel");
	addName("Blarrhgh");
	addName("Xargs");
	addName("Echo");
	addName("Cksum");
	addName("Assembly");
	addName("C");
	addName("C++");
	addName("Simula");
	addName("Modula");
	addName("Java");
	addName("not-a-name");
	addName("semaphore");
	addName("loop");
	addName("breakpoint");
	addName("Operator");
	addName("NULL");
	addName("pointer");
	addName("void pointer");
	addName("overload");
	addName("string");
	addName("semicolon");
	addName("bracket");
	addName("exit");
	addName("main");
	addName(".h");
	addName(".cpp");
	addName("dependency");
	addName("include");
	addName("hash");
	addName("bash");
	addName("crash");
	addName("git");
	addName("multi-core");
	addName("shell");
	addName("printf");
	addName("debug");
	addName("release");
	addName("data stream");
	addName("frustum");
	addName("culling");
	addName("segfault");
	addName("stack overflow");
	addName("buffer overflow");
	addName("cache");
	addName("bottleneck");
	addName("instance");
	addName("viewport");
	addName("render target");
	addName("static");
	addName("update");
	addName("remove");
	addName("allocate");
	addName("deallocate");
	addName("performance");
	addName("std");
	addName("format");
	addName("breakpoint");
	addName("clear");
	addName("box");
	addName("vertex");
	addName("triangle");
	addName("primitive");
	addName("function");
	addName("entity");
	addName("master");
	addName("IDE");
	addName("operating system");
	addName("syscall");
	addName("PCB");
	addName("process");
	addName("thread");
	addName("aligned");
	addName("file");
	addName("handle");
	addName("stack");
	addName("mutable");
	addName("immutable");
	addName("tech");
	addName("pixel");
	addName("voxel");
	addName("polishing");
	addName("what is polishing?");
	addName("chunk");
	addName("data structure");
	addName("spatial");
	addName("event");
	addName("procedure");
	addName("mode");
	addName("G332");
	addName("G403");
	addName("hacker");
	addName("cracker");
	addName("exception");
	addName("array");
	addName("activate");
	addName("enable");
	addName("heap");
	addName("bubble sort");
	addName("stringstream");
	addName("fstream");
	addName("goto");
	addName("jump");
	addName("branch");
	addName("generate");
	addName("caves and buildings");
	addName("engine");
	addName("on the fly");
	addName("compile");
	addName("recompile");
	addName("optimize");
	addName("billboard");
	addName("Solmyr");
	addName("Lord Haart");
	addName("include guard");
	addName("directive");
	addName("concatenate");
	addName("derived");
	addName("polymorphism");
	addName("dynamic binding");
	addName("static binding");
	addName("swap chain");
	addName("device");
	addName("stack pointer");
	addName("assembler");
	addName("linker");
	addName("target");
	addName("status");
	addName("virtual memory");
	addName("garbage collection");
	addName("incremental garbage collection");
	addName("debugger");
	addName("worker thread");
	addName("thread pool");
	addName("system");
	addName("override");
	addName("output");
	addName("input");
	addName("device driver");
	addName("driver");
	addName("memory leak");
	addName("error");
	addName("fail");
	addName("success");
	addName("reference");
	addName("by-reference");
	addName("by-name");
	addName("denominator");
	addName("numerator");
	addName("fraction");
	addName("splitter");
	addName("inverse");
	addName("local");
	addName("global");
	addName("scope");
	addName("run-time");
	addName("compile-time");
	addName("stack trace");
	addName("call stack");
	addName("error list");
	addName("line number");
	addName("utility");
	addName("project");
	addName("Win32");
	addName("x86");
	addName("x64");
	addName("jumper");
	addName("kernel");
	addName("build");
	addName("comment");
	addName("code");
	addName("slash");
	addName("backslash");
	addName("symbol");
	addName("solution");
	addName("problem");
	addName("hotkey");
	addName("call hierarchy");
	addName("hotswap");
	addName("dll");
	addName("object");
	addName("class");
	addName("recursion");
	addName("bit");
	addName("custom name");
	addName("newline");
	addName("out of memory");
	addName("restart");
	addName("define");
	addName("macro");
	addName("forward declaration");
	addName("backward compatibility");
	addName("non-inclusive");
	addName("imperative");
	addName("agile");
	addName("message");
	addName("launch");
	addName("word");
	addName("integer");
	addName("boolean");
	addName("nibble");
	addName("double word");
	addName("32-bit");
	addName("64-bit");
	addName("floating point");
	addName("increment");
	addName("decrement");
	addName("rebuild solution");
	addName("clean");
	addName("failure");
	addName("incomplete");
	addName("invalid");
	addName("unsupported");
	addName("infinite");
	addName("finite");
	addName("please update");
	addName("condition");
	addName("batch");
	addName("timeout");
	addName("skip to finish");
	addName("apply purpose");
	addName("buffer");
	addName("component");
	addName("entity");
	addName("attribute");
	addName("overhead");
	addName("mathematics");
	addName("trade-off");
	addName("callback");
	addName("source code");
	addName("open source");
	addName("deferred rendering");
	addName("abstract");
	addName("backbuffer");
	addName("double-buffering");
	addName("program");
	addName("hardcoded");
	addName("hard-wired");
	addName("programmer");
	addName("unfinished operation");
	addName("matrix");
	addName("algebra");
	addName("quaternion");
	addName("root node");
	addName("export");
	addName("import");
	addName("OBJ");
	addName("clamp");
	addName("collision object");
	addName("step simulation");
	addName("interpolation");
	addName("extrapolation");
	addName("dangling pointer");
	addName("smart pointer");
	addName("unknown");
	addName("unset");
	addName("reset");
	addName("unrecoverable");
	addName("data failed to be");
	addName("system-overload");
	addName("reborn as Maya");
	addName("invaluable token");
	addName("MyClass");
	addName("Hello World");
	addName("quick fix");
	addName("patch");
	addName("version control");
	addName("code evaluation");
	addName("your neighbour");
	addName("interpretor");
	addName("inter-predator");
	addName("failed to load file");
	addName("constant");
	addName("variable");
	addName("member of society");
	addName("edit");
	addName("almost valid");
	addName("Battle In Operating System (BIOS)");
	addName("repository");
	addName("tester");
	addName("invisible swan");
	addName("proprietary");
	addName("illegal operation");
	addName("try to score");
	addName("not tested");
	addName("as-is");
	addName("no guarantee");
	addName("self-inflicted compile error");
	addName("boost");
	addName("helper function");
	addName("wrapper class");
	addName("not a number");
	addName("error code -1");
	addName("error code");
	addName("same as usual");
	addName("render");
	addName("skip action");
	addName("take branch");
	addName("escape");
	addName("unordered sequence");
	addName("briefer-than-life");
	addName("call on fail");
	addName("railway on ice");
	addName("injection");
	addName("synchronous");
	addName("asynchronous");
	addName("unexpected behavior");
	addName("tabulator");
	addName("unstable");
	addName("sleep");
	addName("oversleeping sleep");
	addName("first in");
	addName("last out");
	addName("infinite queue");
	addName("nontrivial");
	addName("unsortable");
	addName("distorted");
	addName("loop-the-loop");
	addName("race condition");
	addName("indecipherable");
	addName("capacitor");
	addName("one");
	addName("zero");
	addName("block");
	addName("free space");
	addName("BAADF00D");
	addName("0x0");
	addName("hexadecimal decimal");
	addName("hex to the seventh power");
	addName("power of two");
	addName("reversible operation");
	addName("irreversible condition");
	addName("hardware");
	addName("software");
	addName("firmware");
	addName("shareware");
	addName("freeware");
	addName("bloatware");
	addName("malware");
	addName("shortest path");
	addName("Dijkstra's algorithm");
	addName("divide and conquer");
	addName("greedy algorithm");
	addName("algorithm");
	addName("vector");
	addName("tensor");
	addName("scalar");
	addName("ordo N");
	addName("notation");
	addName("gimbal lock");
	addName("boot");
	addName("reboot");
	addName("encryption");
	addName("anonymous hacker");
	addName("uninvited player");
	addName("socket");
	addName("northbridge");
	addName("southbridge");
	addName("London bridge");
	addName("demo");
	addName("tech demo");
	addName("scientist with laser");
	addName("run-and-gun");
	addName("interrupt");
	addName("exception handler");
	addName("unhandled exception");
	addName("there is a fire downtown");
	addName("sound the alarm");
	addName("wide character");
	addName("do this while that");
	addName("break on wake");
	addName("wake on sleep");
	addName("stash the hash");
	addName("public key");
	addName("mutex lock");
	addName("monitor");
	addName("auto-update");
	addName("our new software");
	addName("piece of furniture");
	addName("cancelled restart sequence");
	addName("dead battery");
	addName("acid");
	addName("wireless wires");
	addName("hired wire");
	addName("undo hinges");
	addName("unknown operator");
	addName("unsupported operation");
	addName("deprecated");
	addName("meeting");
	addName("massed standards");
	addName("supended");
	addName("gardener with pruning shears");
	addName("weird amount of time");
	addName("bottle of data");
	addName("current line");
	addName("console");
	addName("clowny novelty");
	addName("megaton");
	addName("byte");
	addName("kilobyte");
	addName("megabyte");
	addName("gigabyte");
	addName("terabyte");
	addName("extraction point");
	addName("interface");
	addName("valid");
	addName("inline");
	addName("online");
	addName("offline");
	addName("repeat buffer");
	addName("cross the dot");
	addName("axis");
	addName("quick save");
	addName("lines of code");
	addName("offset");
	addName("alignment");
	addName("terms of agreement");
	addName("terms of service");
	addName("terms of use");
	addName("swap twice");
	addName("loader");
	addName("shader");
	addName("hlsl");
	addName("loop unrolling");
	addName("normal");
	addName("look");
	addName("effect");
	addName("seed");
	addName("random");
	addName("rand");
	addName("join");
	addName("noname");
	addName("null-terminated");
	addName("storage");
	addName("cloud");
	addName("directional light");
	addName("pale shadow");
	addName("clone");
	addName("bot");
	addName("anti");
	addName("piggy");
	addName("hijacked connection");
	addName("live or unlife");
	addName("half the amount");
	addName("not quite so");
	addName("in actuality");
	addName("inactual");
	addName("unemployed actor");
	addName("unemployed programmer");
	addName("pay-by-day");
	addName("bound resource");
	addName("unsaturated");
	addName("pipeline");
	addName("staged crash");
	addName("tablespoon");
	addName("rock");
	addName("XKILL");
	addName("YKILL");
	addName("Barney");
	addName("The man himself");
	addName("INT_MAX");
	addName("return on fail");
	addName("colon-separated");
	addName("dispatch");
	addName("path");
	addName("rinse and repeat");
	addName("trial and error");
	addName("trial version");
	addName("unregistered copy");
	addName("30 days left");
	addName("do not answer this email");
	addName("memory address");
	addName("out-of-range");
	addName("disarray");
	addName("reorganize");
	addName("evil crystal");
	addName("get used to");
	addName("on/off-switch");
	addName("transistor");
	addName("modifier");
	addName("radius");
	addName("unnoticeable difference");
	addName("I beg to differ");
	addName("in effect");
	addName("there is no such thing");
	addName("monster on second floor");
	addName("legitimate user");
	addName("last error: not found");
	addName("network administrator");
	addName("empty toolbox");
	addName("assert");
	addName("crazy man on the street");
	addName("index of value");
	addName("assumption");
	addName("resume latest assumption");
	addName("xyzzy");
	addName("clear buffer");
	addName("week-old branch");
	addName("undefined behavior");
	addName("performance measurement");
	addName("perform action");
	addName("oddly so");
	addName("to the next level");
	addName("press start");
	addName("please register");
	addName("unrecognized command");
	addName("0 is equal to 0");
	addName("valid statement");
	addName("statement of uncertainty");
	addName("I did not see that");
	addName("only imagination");
	addName("imaginatively");
	addName("frostbar snack");
	addName("inner workings");
	addName("sensemaking");
	addName("creator of process");
	addName("ray-foot");
	addName("max flight height");
	addName("over the underpass");
	addName("makeshift feature");
	addName("inverted reality");
	addName("collection of conscience");
	addName("undigested dilemma");
	addName("Guybrush Threepwood");
	addName("worse than ever");
	addName("enemy spotted");
	addName("timestamp");
	addName("search and replace");
	addName("current document");
	addName("entire solution");
	addName("just set it to 0");
	addName("deliver, deliver, deliver");
	addName("road to Denver");
	addName("backface culling");
	addName("deadline 2013-03-25");
	addName("boats in outer space");
	addName("highly trained professional");
	addName("who");
	addName("I");
	addName("that is not what I said");
	addName("desktop");
	addName("selective memory");
	addName("memory of happenstance");
	addName("heap corruption");
	addName("Face recognition: Go away.");
	addName("repeat action");
	addName("jokes, recursion jokes");
	addName("joker of poker");
	addName("deliberate crash-on-boot");
	addName("blink twice for yes");
	addName("smooth surface");
	addName("sound off");
	addName("fire in the hole");
	addName("breathe and suffer");
	addName("blast doors");
	addName("inadequate solution");
	addName("blinded by the light");
	addName("bleeding edge");
	addName("save and accept");
	addName("proper footwear");
	addName("Amsterdam");
	addName("browser");
	addName("print error");
	addName("inverted normals");
	addName("empty help file");
	addName("cmd");
	addName("faulty recover mechanism");
	addName("press any key");
	addName("please insert CD");
	addName("cannot close file");
	addName("assertion failed");
	addName("undeniably so");
	addName("just another index");
	addName("world has gone missing");
	addName("regroup and fare well");
	addName("future head of command");
	addName("joint operation");
	addName("add descriptor");
	addName("document this");
	addName("undocumented");
	addName("todo");
	addName("yesterday's ado");
	addName("red, green, blue");
	addName("convex hull");
	addName("collision detection");
	addName("blacken");
	addName("back-end");
	addName("front-end");
	addName("model");
	addName("whitespace");
	addName("carrage return");
	addName("line feed");
	addName("unable to swap buffers");
	addName("character missing a head");
	addName("MD5");
	addName("function gone missing");
	addName("link error");
	addName("unresolved external");
	addName("internal corruption");
	addName("applicable");
	addName("or else never do");
	addName("this little light of mine");
	addName("length of sample");
	addName("sampler state");
	addName("absolute value");
	addName("bones");
	addName("skeletal animation");
	addName("standard procedure");
	addName("default");
	addName("reset to random");
	addName("humble gesture of command");
	addName("one-handed banker wielding two-handed axe");
	addName("bloom");
	addName("indeed not");
	addName("angry consultant");
	addName("new message");
	addName("otherwise skip");
	addName("refactorization");
	addName("patron of establishment");
	addName("12 MHz");
	addName("kernel panic");
	addName("picknick in cellar");
	addName("on paper for now");
	addName("divine agreement");
	addName("absolutely not");
	addName("compile objection");
	addName("wastefull");
	addName("basket of flowers");
	addName("underutilized");
	addName("made the cut");
	addName("aborted operation");
	addName("does not compute");
	addName("recalculation");
	addName("recalcitrant");
	addName("burn the code");
	addName("tried");
	addName("deflatable rivet");
	addName("disassembled");
	addName("graphics card on the wall");
	addName("spin out of control");
	addName("Switzerland");
	addName("unwanted occlusion");
	addName("self-occlusion");
	addName("undid most of it");
	addName("high-speed");
	addName("motherboard");
	addName("static electricity");
	addName("distance table");
	addName("make it happen");
	addName("easter egg");
	addName("frayed around the edges");
	addName("sons of coding");
	addName("folder");
	addName("find");
	addName("� SUN-STAR �");
	addName("trip and fall");
	addName("Destination @");
	addName("division by zero");
	addName("unnamed label");
	addName("warning: no warnings");
	addName("beware of warnings");
	addName("default studio");
	addName("CAPS LOCK");
	addName("minor annoyance");
	addName("source and destination");
	addName("quantum computer");
	addName("manager wearing a tie");
	addName("standard bug");
	addName("Windows 8");
	addName("Twinsen");
	addName("Nitro-Meca-Penguin");
	addName("implementor");
	addName("remind me later");
	addName("mentally prepared");
	addName("more than average");
	addName("dump the chips");
	addName("chipset");
	addName("hardly working");
	addName("keep doing that other thing");
	addName("is this my name?");
	addName("ironic ironing board");
	addName("moronic oxymoron");
	addName("attack with sharpened steel");
	addName("Erathia");
	addName("well I never");
	addName("clean it inside out");
	addName("try recompiling");
	addName("rise and shine");
	addName("floating lunch money");
	addName("rounding error");
	addName("diversion of attention");
	addName("risk factor");
	addName("fraudulent scenario");
	addName("insane amount");
	addName("gamepad");
	addName("Re-Volt");
	addName("eco mode");
	addName("Z-order assignment");
	addName("already in use");
	addName("access denied");
	addName("access granted");
	addName("no more people");
	addName("glow");
	addName("exporter");
	addName("muzzle flash");
	addName("please fill out this form");
	addName("ice cubes hit the floor");
	addName("read and write");
	addName("unconditioned");
	addName("ASCII");
	addName("character encoding");
	addName("encoder");
	addName("public meeting");
	addName("wholegrain and wheat");
	addName("may I disagree");
	addName("tripple x");
	addName("skinned mesh");
	addName("lookout tower");
	addName("unselected selection");
	addName("mute all");
	addName("never fear");
	addName("the old backup");
	addName("backup?");
	addName("tumbleweed");
	addName("rasterization");
	addName("quota");
	addName("partition");
	addName("unfreed memory");
	addName("memory refresh");
	addName("cache miss");
	addName("compatibility disagreement");
	addName("unsign here");
	addName("a plate that reads \"No\"");
	addName("upper limit");
	addName("reallocation");
	addName("token taken");
	addName("malloc");
	addName("free");
	addName("C run-time library");
	addName("right-handed");
	addName("coordinate system");
	addName("coordination inability");
	addName("failure as a pirate");
	addName("skin of an oyster");
	addName("shine the shoehorn");
	addName("sledgehammer with nails");
	addName("closed coffin");
	addName("smoke particle");
	addName("plugin");
	addName("front-side bus");
	addName("forlorn castles");
	addName("cascading something");
	addName("replayability");
	addName("normalized");
	addName("under certain conditions");
	addName("do not enter");
	addName("Python");
	addName("Lua");
	addName("automatic automation");
	addName("restore data");
	addName("graceful recovery");
	addName("Blender");
	addName("shredder");
	addName("rebound");
	addName("weight painting");
	addName("cabin in the woods");
	addName("text message");
	addName("funny sound");
	addName("just because");
	addName("returned function");
	addName("square root");
	addName("verbosity");
	addName("inappropriate flag");
	addName("properties");
	addName("spec");
	addName("galaxy treatment");
	addName("fried ham");
	addName("strong typing");
	addName("weak typing");
	addName("pop a balloon");
	addName("push");
	addName("pulley");
	addName("digging a hole");
	addName("redraw scene");
	addName("underlying weakness");
	addName("sailing southeast");
	addName("crowded square");
	addName("missing a soul");
	addName("if more or less even");
	addName("congress or progress");
	addName("regressive statement");
	addName("to render unusable");
	addName("right there");
	addName("may I say never");
	addName("happy");
	addName("pristine prime number");
	addName("tilly tally ho");
	addName("gigantic antic");
	addName("rusty anchor");
	addName("a bird flew away");
	addName("nested loops");
	addName("the eagle");
	addName("continue down there");
	addName("break the silence");
	addName("lunch satisfaction");
	addName("free lunch");
	addName("crash, or just a bad day");
	addName("constantly trying");
	addName("not figuratively");
	addName("bleep bloop");
	addName("beep");
	addName("brainless");
	addName("mack the knife");
	addName("corollary");
	addName("followed by silence");
	addName("queue and wait");
	addName("hello hurray");
	addName("pushed onto stack");
	addName("random success");
	addName("strange symptom");
	addName("shared cache buffer");
	addName("edge-preserving plane tool");
	addName("outnumbered enum");
	addName("spontaneous combustion");
	addName("there exists such people");
	addName("good example");
	addName("souped-up Jitney");
	addName("large giveaway of nothing");
	addName("bipartite dual bicyle");
	addName("new bounding box");
	addName("no logo");
	addName("noclip");
	addName("low-cost no-cost");
	addName("animal exhibition");
	addName("performance-friendly");
	addName("missing indices");
	addName("I'm just browsing ");
	addName("R8 cat");
	addName("ten minutes left");
	addName("seasoned reasoning");
	addName("play anywhere");
	addName("transmitter");
	addName("blown to smithereens");
	addName("stop and execute");
	addName("<>|God|<>");
	addName("Jesus");
	addName("remarkably plain wrong");
	addName("cane-wielding forester");
	addName("seems right");
	addName("not so");
	addName("reanimated skeleton");
	addName("furniture out, insurance money in");
	addName("put that thing down, now!");
	addName("look, run and gun");
	addName("cluttered interface");
	addName("merge conflict");
	addName("compressed");
	addName("lied and died");
	addName("weekend holdout");
	addName("charity and goodwill");
	addName("hand over your weapon");
	addName("stop and think");
	addName("collaboration effort");
	addName("useless garbage");
	addName("window of opportunity");
	addName("recover");
	addName("unexpected permission");
	addName("enter");
	addName("active window");
	addName("window cleaning");
	addName("disappeared loyal follower");
	addName("improper use of symbol");
	addName("a sudden sound");
	addName("hard to remember");
	addName("enchanted ring");
	addName("peak flow rate");
	addName("optimal condition");
	addName("gold gatherer");
	addName("miner with beard");
	addName("for a piece of food");
	addName("the previous reiteration");
	addName("remember our old car max");
	addName("bereaved champion");
	addName("come to a closure");
	addName("don't come closer");
	addName("fisherman's rod");
	addName("hook, line and sinker");
	addName("sunken boat aflot");
	addName("something is not right");
	addName("dum-dum-dum");
	addName("bunny-jumper");
	addName("Rockefeller's torn wallet");
	addName("system of rice");
	addName("two thousand of something");
	addName("int or float");
	addName("row-major");
	addName("Tony Rocky Horror");
	addName("double the effort");
	addName("right on time");
	addName("proportionality");
	addName("unexpected end of line found");
	addName("missing semicolon");
	addName("enjoying yourself");
	addName("progress tracker");
	addName("finally here");
	addName("won the jackpot");
	addName("one in a million");
	addName("secret nickname");
	addName("we have a winner");
	addName("for the love of God");
	addName("ogre in tent");
	addName("claim to fame");
	addName("precariously precautious");
	addName("sensitivity");
	addName("hand cannon");
	addName("friday");
	addName("I wonder why it crashes");
	addName("televised appearance");
	addName("early adopter");
	addName("testing environment");
	addName("set head to next");
	addName("It's O.K. I think");
	addName("monkey in a n�glig�e");
	addName("pick another color");
	addName("plaster apron");
	addName("pencil sharpener");
	addName("I wonder");
	addName("comparision outside set");
	addName("empty-handed");
	addName("sea you underwater");
	addName("my view");
	addName("fair enough");
	addName("as long as its a word");
	addName("name or not");
	addName("Frank D. Luna");
	addName("trailing zero");
	addName("backtrack");
	addName("G�ran Fries");
	addName("gcc");
	addName("MSVCP110.dll");
	addName("digital scrumboard");
	addName("Danjer Cove");
	addName("lossless loser");
	addName("it's time to choose");
	addName("planet workload");
	addName("overslept overclocking");
	addName("Island CX");
	addName("The undergas");
	addName("wirt's leg");
	addName("first day of summer");
	addName("do that later");
	addName("fundamental flaw");
	addName("metadata");
	addName("lawbreaker");
	addName("jawbreaker");
	addName("the one");
	addName("ore and mercury");
	addName("my companion seemed to know the way");
	addName("to arms and feet");
	addName("captain's locker");
	addName("mundane-looking farmer");
	addName("dreaming processor");
	addName("landscape architect");
	addName("paintbrush of serenity");
	addName("painter of two minds");
	addName("use your experience");
	addName("trigger happy");
	addName("Sputnik");
	addName("I digress");
	addName("previous transgressions");
	addName("channel open");
	addName("something on your mind?");
	addName("identify target");
	addName("boots of blinding speed");
	addName("transmit coordinates");
	addName("dishevelled appearance");
	addName("malevolent virus");
	addName("magnitude of scale");
	addName("flower of ipecac");
	addName("chilling flamethrower");
	addName("chainsaw and lawn mower");
	addName("pliers and blowtorch");
	addName("child-abducting demon");
	addName("down to earth and back again");
	addName("for each and every year");
	addName("smell of rain");
	addName("fear of anxiety");
	addName("anthrax powder");
	addName("wrong pivot");
	addName("last of the living");
	addName("flip of a coin");
	addName("I said I was sorry");
	addName("fork and spoon");
	addName("colorless rainbow");
	addName("brainbow");
	addName("taste cold steel");
	addName("eat lead");
	addName("element of uncertainty");
	addName("append");
	addName("prepend");
	addName("big game hunting");
	addName("hunter");
	addName("no can do");
	addName("repeat after me");
	addName("begging the question");
	addName("objection your honor");
	addName("golden gecko pelt");
	addName("utility belt");
	addName("working box");
	addName("translation from this to that");
	addName("working as intended");
	addName("train of thought");
	addName("miscellany");
	addName("problem arms");
	addName("fewer suits");
	addName("framed characters");
	addName("to whom it may concern");
	addName("straight up honest truth");
	addName("stranger danger");
	addName("ferry ticket");
	addName("token of gratitude");
	addName("game over");
	addName("overlap");
	addName("final lap");
	addName("grocery store");
	addName("tick rate");
	addName("as I was saying");
	addName("banhammer smash");
	addName("calliope");
	addName("alpha");
	addName("beta");
	addName("delta");
	addName("Cyrus the Virus");
	addName("angle grinder");
	addName("normalized quaternion");
	addName("bad object");
	addName("It's in the Book");
	addName("Book of Job");
	addName("Oddjob");
	addName("paranoid void");
	addName("voracity");
	addName("Mount Wormore");
	addName("wreck havock");
	addName("down by the street");
	addName("last man standing");
	addName("allocator");
	addName("REM");
	addName("right on track");
	addName("in good standing");
	addName("by the Gods");
	addName("enable mouse look");
	addName("flipswitch");
	addName("the magic border");
	addName("not even close to being a problem");
	addName("shell script");
	addName("ballot");
	addName("quantum leap");
	addName("the higher quantum");
	addName("brain rating");
	addName("humanity");
	addName("solver");
	addName("raid by night");
	addName("pinned down");
	addName("windbreaker");
	addName("fine leather jackets");
	addName("hazardous material");
	addName("new int");
	addName("guarding guard");
	addName("bow and arrow");
	addName("Flyer 1");
	addName("out of ammo");
	addName("sense modifiction");
	addName("treasure map");
	addName("selection of section");
	addName("candyland");
	addName("paper art");
	addName("reserved vector");
	addName("all ways");
	addName("cover-up operation");
	addName("major breakthrough");
	addName("commander");
	addName("make");
	addName("mixed case");
	addName("half-eaten birthday cake");
	addName("burning candles");
	addName("wreck of Titanic");
	addName("find me if you can");
	addName("hit or miss");
	addName("funny bone");
	addName("flintstone authority");
	addName("unauthorized");
	addName("broken glass eye");
	addName("spectrum of pain");
	addName("low-level");
	addName("high-level");
	addName("metro gremlin");
	addName("keep single");
	addName("stay put");
	addName("kenyan coffee");
	addName("Kalaydean whah");
	addName("www.xkill.se");
	addName("template system");
	addName("plenty of balloons");
	addName("thrashing");
	addName("Deep Blue");
	addName("HAL 9000");
	addName("editor's choice");
	addName("just a tool");
	addName("run ran crash");
	addName("cultivating garlic");
	addName("gas duster");
	addName("logical reasoning xor");
	addName("version outdated");
	addName("seamless integration");
	addName("unexpected attendance");
	addName("aforementioned but forgotten");
	addName("had a lucky start");
	addName("show the way");
	addName("implicitly correct");
	addName("on the right track with wrong ticket");
	addName("roses in bloom");
	addName("King Richard");
	addName("Ebas Neue");
	addName("typecast");
	addName("missing before, found now");
	addName("O missing before K");
	addName("stable but quaint");
	addName("nefarious deed");
	addName("grizzly and his buffer");
	addName("I understand arrows");
	addName("laid-back backpack");
	addName("heavy cavalry");
	addName("alternative operator");
	addName("modifiable lvalue");
	addName("yarn and thread");
	addName("unique constant buffer");
	addName("exactly the same");
	addName("not exactly sane");
	addName("verge of extinction");
	addName("hammer and nail");
	addName("to no avail");
	addName("bitten by snake");
	addName("deferred context");
	addName("multi-layered cake");
	addName("weed indeed");
	addName("Singkil");
	addName("list of stuff");
	addName("stuffed animal");
	addName("non-renderable interface");
	addName("Internet access");
	addName("mushroom iterator");
	addName("once upon a time");
	addName("free for all");
	addName("aerial runner");
	addName("infinite by default");
	addName("needle in a stack");
	addName("levitation");
	addName("error-prone");
	addName("DNA repair");
	addName("sources of damage");
	addName("cycle steal");
	addName("simple solution");
	addName("I think so, definitely");
	addName("referred to as two");
	addName("executable");
	addName("excusable");
	addName("rethinkably done");
	addName("possible loss of data");
	addName("consider it done");
	addName("c1xx");
	addName("parameter mayhem");
	addName("argument sketch");
	addName("talkactive parrot");
	addName("keymapper");
	addName("end of user");
	addName("frail ladder");
	addName("[armored tank]");
	addName("one-ear");
	addName("supposed to know");
	addName("twitchy");
	addName("pervertex");
	addName("sleeping thread");
	addName("cap the rendering thread");
	addName("selling snow");
	addName("freezer");
	addName("joy");
	addName("false-positive");
	addName("Type I and type II errors");
	addName("buildbot winslave");
	addName("bootstrapper");
	addName("gunpowder wagon");
	addName("bamboo sword");
	addName("on a tight leash");
	addName("call to arms");
	addName("relaxed and lazy");
	addName("peak rate");
	addName("weapon house");
	addName("visiting magnetism");
	addName("went out of control from there");
	addName("shuffle-face");
	addName("original origo");
	addName("juniper berry");
	addName("spatial suggestion");
	addName("hidden reference");
	addName("inside a barrel");
	addName("crooky crocodile");
	addName("locked safe");
	addName("timers and threads");
	addName("make ends meet");
	addName("syntax error");
	addName("flying ladybug");
	addName("sholess shoemaker");
	addName("live wire");
	addName("rain dance");
	addName("dismembered honorary member");
	addName("chairman of the board");
	addName("dimly lit apartment");
	addName("sticky envelope");
	addName("chain of command");
	addName("law-abider");
	addName("monday morning");
	addName("time of day");
	addName("post-it");
	addName("letter with one letter: t");
	addName("no more waste of space");
	addName("wasps and honey");
	addName("sour vegetable");
	addName("success emulator");
	addName("bind pose");
	addName("bit by bit");
	addName("firelighter");
	addName("wishing upon a star");
	addName("cold fish head");
	addName("words of advice");
	addName("final excuse");
	addName("utter devastation");
	addName("pyramid burried in sand");
	addName("geometry shader");
	addName("big monster");
	addName("citizen of Venice");
	addName("drought season");
	addName("ebb tide");
	addName("a week later");
	addName("suddenly nothing");
	addName("will to power");
	addName("another thread");
	addName("in charge");
	addName("context switch");
	addName("tractor");
	addName("scallywag");
	addName("no-brainer");
	addName("cat wizard");
	addName("pick a card, any card");
	addName("floating-point precision");
	addName("carrot cake");
	addName("shine light on the great");
	addName("text whisper");
	addName("code washing");
	addName("compelling screen capture");
	addName("autocomplete");
	addName("gadgetry");
	addName("retired clergyman");
	addName("hightailing motorized vehicle");
	addName("goblet of water");
	addName("collection of tidbits");
	addName("I found it, and gave it away");
	addName("tangible difference");
	addName("hello alloy");
	addName("rubber piggy");
	addName("cheap piano");
	addName("complaintiff");
	addName("judgy");
	addName("common trait");
	addName("a nickel for a dime");
	addName("dining room table");
	addName("staff working hours");
	addName("ability to shiver");
	addName("logarithmical exponentiality");
	addName("garden variety");
	addName("burning firearms");
	addName("derailed freight train");
	addName("iffy and only iffy");
	addName("bitwise wisdom");
	addName("git magic");
	addName("self-thinking trustee");
	addName("globlur");
	addName("insufficient instrumentation");

	addName("ERROR_SUCCESS");
	addName("bytes want to be free");
	addName("send spike");
	addName("a hint of madness");
	addName("a tint of rust");
	addName("sinister indeed");
	addName("Neuromancer");
	addName("sun stealer");
	addName("devil's pawn");
	addName("sheer excellence");
	addName("taste the graphics");
	addName("Beezow doo-doo");
	addName("zopittybop-bop-bop");
	addName("murder and mayhem");
	addName("fancy box");
	addName("knowledge brings fear");
	addName("incoherent response");
	addName("memleak");
	addName("elderberries");
	addName("spiteful crow");
	addName("crackpot");
	addName("graceful feature degradation");
	addName("nothing but beans");
	addName("ulterior motives");
	addName("spam");
	addName("zero feedback");
	addName("remove before release");
	addName("a matter of taste");
	addName("tastes like chicken");
	addName("dubious character");
	addName("mad scientist");
	addName("evil inside");
	addName("poor Yorick");
	addName("he's back");
	addName("the missing link");
	addName("Dr. Moreau");
	addName("positronic brain");
	addName("brain in a jar");
	addName("Wintermute");
	addName("polysyllabic moniker");
	addName("Excelsior");
	addName("Magnificus");
	addName("Yuggoth");
	addName("is this reality?");
	addName("space monkey");
	addName("highly trained monkey");

	addName("inexpensive payback time");
	addName("better safe than gory");
	addName("open time window");
	addName("update frequency");
	addName("translation of incubation");
	addName("90 degree rotation");
	
	//C++ keywords
	addName("alignas");
	addName("alignof");
	addName("and");
	addName("and_eq");
	addName("asm");
	addName("auto");
	addName("bitand");
	addName("bitor");
	addName("bool");
	addName("break");
	addName("case");
	addName("catch");
	addName("char");
	addName("char16_t");
	addName("char32_t");
	addName("class");
	addName("compl");
	addName("const");
	addName("constexpr");
	addName("const_cast");
	addName("continue");
	addName("decltype");
	addName("default");
	addName("delete");
	addName("do");
	addName("double");
	addName("dynamic_cast");
	addName("else");
	addName("enum");
	addName("explicit");
	addName("export");
	addName("extern");
	addName("false");
	addName("float");
	addName("for");
	addName("friend");
	addName("goto");
	addName("if");
	addName("inline");
	addName("int");
	addName("long");
	addName("mutable");
	addName("namespace");
	addName("new");
	addName("noexcept");
	addName("not");
	addName("not_eq");
	addName("nullptr");
	addName("operator");
	addName("or");
	addName("or_eq");
	addName("private");
	addName("protected");
	addName("public");
	addName("register");
	addName("reinterpret_cast");
	addName("return");
	addName("short");
	addName("signed");
	addName("sizeof");
	addName("static");
	addName("static_assert");
	addName("static_cast");
	addName("struct");
	addName("switch");
	addName("template");
	addName("this");
	addName("thread_local");
	addName("throw");
	addName("true");
	addName("try");
	addName("typedef");
	addName("typeid");
	addName("typename");
	addName("union");
	addName("unsigned");
	addName("using");
	addName("virtual");
	addName("void");
	addName("volatile");
	addName("wchar_t");
	addName("while");
	addName("xor");
	addName("xor_eq");
}
void NameGenerator::addName(std::string name)
{
	names.push_back(name);
}

#pragma warning(disable: 4996)

std::string NameGenerator::getName()
{
	// Reset names if all have been picked
	if(names.size() <= 0)
		resetNames();

	// Pick random name
	int numNames = names.size();
	int index = Math::randomInt(0, numNames-1);
	std::string name = names.at(index);

	// Remove name using Swap-Trick
	names.at(index) = names.back();
	names.pop_back();


	// EASTER EGG
	static bool wasShown = false;
	static int numStarts = 0;
	if(numStarts < 4)
		numStarts++;
	else
	{
		if(!wasShown)
		{
			const int chance = 25;
			if(Math::randomInt(0, chance) == chance)
			{
				wasShown = true;
				name = getenv( "USERNAME" );
			}
		}
	}
		

	// Return name
	return name;
}

void NameGenerator::reset()
{
	resetNames();
	resetColors();
}

void NameGenerator::resetColors()
{
	colors.clear();

	//
	// Add colors
	//

	addColor(Float3(0.0f, 1.0f, 0.0));
	addColor(Float3(1.0f, 0.0f, 0.0));
	addColor(Float3(0.0f, 1.0f, 1.0));
	addColor(Float3(1.0f, 0.4f, 0.0));
	addColor(Float3(0.7f, 0.0f, 1.0));
}

void NameGenerator::addColor( Float3 color )
{
	colors.push_back(color);
}

Float3 NameGenerator::getColor()
{
	// Reset colors if all have been picked
	if(colors.size() <= 0)
		resetColors();

	// Pick random color
	int numColors = colors.size();
	int index = Math::randomInt(0, numColors-1);
	Float3 color = colors.at(index);

	// Remove name using Swap-Trick
	colors.at(index) = colors.back();
	colors.pop_back();


	// Return name
	return color;
}
