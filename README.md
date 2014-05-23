SuperKoalio_Cocos2D-X_3.1
=========================

A port of the Ray Wenderlich tutorial to Cocos 2D-X

This is a port of the project located at: http://www.raywenderlich.com/15230/how-to-make-a-platform-game-like-super-mario-brothers-part-1 and http://www.raywenderlich.com/15267/how-to-make-a-platform-game-like-super-mario-brothers-part-2 to the most recent version of Cocos 2D-X, 3.1 rc0.

I undertook this project largely due to the dearth of updated tutorials for Cocos 2D-X, and my desire to work on a simple 2D platformer as well as looking for a reason to brush up on my C++ skills. In total it took me maybe a work day and a half to port this over and get it running without any major bugs. That being said there are a few things potential users of this code should be aware of:

* I have largely retained the naming scheme of the original sample, however I did rename a number of variables and functions to fit a more professional coding standard. So instead of seeing "TMXTiledMap* map" you will see: "TMXTiledMap* m_pMap", there's no real functional difference but after 5 years of professional programming I can't help but give things more meaningful/cleaner names. That being said, I largely left a lot of the "bad" practices of the original sample in tact (magic numbers galore!) so it shouldn't be too hard to read the tutorial and this code and understand what everything does.

* The original sample made use of a couple of iOS only math functions, CGRectInset, CGRectOffset, and CGRectIntersection. There's no standard library replacement for these functions, and the Cocos2D-X Rect class has not offered up replacements either. For the sake of simplicity I simply did the math these functions would do right where they were called. For a real project I would highly suggest that the reader add a header file that defines these, or better yet add them to the Rect class.

* Along the same lines, there are no direct replacements for NSArray, NSMutableArray and NSDictionary. I replaced the first two with std::vector and the second with a simple struct that can be found defined at the top of GameLevelLayer.h. Certainly this is not the only way of solving the issue but for my purposes it worked. It is also worth noting the code at the end of getSurroundingTilesAtPosition, the swap functions available for NSArrays are not present for std::vector, so to make it explicit I did things in a more brute force/clunk way. There are almost certainly better ways of handling this.

* Another slight change I made was to not have the Player class inherit from the Sprite class. This is simply because I was having issues with Cocos2D-X and the way it creates objects, I was getting some really weird inheritance issues. The solution is probably something incredibly simple but given the lack of documentation and my own rustiness I decided to press on in a different direction.

* As a final note, the sample was apparently set up to run in portrait mode, however when I generated this project with the Cocos2D-X tools it apparently defaulted to landscape. As a result the controls are kind of wonky and the the level is placed half way up the screen instead of at the bottom. I might actually fix this, but for now it is left as an exercise for you the reader.

This code is presented "As-is", I don't really have the ability to offer much email support. For questions on the algorithms/logic of this I highly suggest reading the two articles linked at the top of this document as they explain them quite well! All art and music belong to their original creators and were not created by me. I highly recommend consulting the original articles again for more information on them. I have only tested the iOS and Mac builds of this project, so my apologies if it doesn't work on Windows/Android/Tizen etc.
