This is a modified version of the classic Hangman game, which I created as part of my final intro Java coding course.

What makes this version different than normal Hangman is that the user is playing against the CPU, and the CPU has the ability to "cheat".

Essentially, the CPU maintains a list of every word in the English language (found in dictionary.txt). Instead of storing a specific word that the player
attempts to guess, the CPU stores the entire list of words. When the player enters a letter to guess, the CPU creates multiple new lists of "word families":
groups of words where that letter appears in the exact same location(s) within the words. Then, the CPU chooses the word family with the most words in it 
and uses that as its "dictionary" for the next letter guess.

In short, the CPU does not choose the word that the player is trying to guess until there is only one word left that matches the letters the player has guessed,
and usually the player runs out of guesses before this can happen.

Moral of the story: don't trust computers.