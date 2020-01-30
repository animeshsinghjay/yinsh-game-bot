Yinsh Game-Bot
----------
We built a bot which was capable of running true ply 5 (without any selected expansion of good
nodes). We implemented mini-max along with alpha beta pruning. We also sorted the expansion
of internal nodes according to our evaluation function, used hashing for fast evaluation and
implemented killer move. In order to make it reach 5-ply we optimised the functions which were
the slowest step, according to time analysis. We also implemented iterative deepening for
getting to more depths as the game progressed. We implemented random starts in the
beginning and never ditched it :)  

1. Changes in minimax  

* We implemented minimax with alpha beta pruning. Sorting the internal nodes and
expanding them in this order helped us prune many parts provably. We used our
evaluation function to evaluate all our children, sorted them according to this
value, and descended to the children in this order.  
* We also hashed the entire board using Zobrist hashing. This helped us evaluate
the board fast over successive searches of Iterative deepening as it was faster to
look up the hash table than to evaluate the board. This would also work as a
transposition table, but we didn’t see much improvement as the number of
duplicate nodes at a certain level were low.  
* We added two killer moves at each depth, which when applicable would lead to
faster pruning.  
* We also used iterative deepening. We started from a depth of 2, and calculated
the time that minimax run at that depth took. The time taken at the successive
depth would increase by a factor of the branching factor, and we approximated
the branching factor using the number of rings as an indicator. We multiplied the
maximum of the number of our rings and others by 10 to approximate the time
that the next depth would take. If this was found to be low, we moved down to the
next depth. This helped us achieve depth 5 at the start of the game, but as the
game progressed, we were hitting depths of 7, and sometimes even 11. Also, this
made it reach different depths on different computers according to their speed.  

2. Optimizations. 
* We implemented a reverse move after each move in the minimax tree. So, when
we were descending in the depth first search, we would not need to copying the
entire board configuration and could reach it by using the reverse move as
“previous pointers”.  
* During time analysis, we realized that the function which finds if there are
consecutive five markers was called many times and was taking more than 50%
of the execution time. Instead of traversing the whole board to find these rows,
we just checked the markers which changed colour and thus did it in almost
constant time.  
3. Evaluation Function  
For evaluating the goodness of a state we extracted features like continuous_2,
continuous_3, going up to continuous_5. We had maintained the number of my_markers and
other_markers as a part of the state and used them as features too. Besides this, we also
differentiated between markers that were “threatened” by the opponent's rings in an immediate
move and those that were “secure”. So we also had features like secure_continuous_2,
sec_cont_3 going up to sec_cont_5. These features had much more weight than the general
continuous features. We counted these for both myself and my opponent and my evaluation
was symmetric i.e the weights for the features of my opponent were negatives of the weight I
had. For the same number of rings remaining our bot had no preference for being aggressive or
defensive. When we were losing it played defensively and when we were winning it played
aggressively. Another special thing about the features was that we treated our RINGS as secure
markers. The reason is that if that ring were moved from there it would leave behind a marker
and it was secure because at the moment there is a ring there. This meant that my extraction
treated (my_marker,my_marker,my_ring,my_marker) as a sequence of 4.  