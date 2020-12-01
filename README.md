# Vostok AV GitHub Repository
This is the repo for the arduino code of the avionic

## Beginner's guide

Github allows members of a project to all work on a same project. To do this, a common repository (repo) is created and stored here. Now
each person who wants to work on the project will take the current version of the project (called *pulling*) and work on it locally 
(on his own PC). When done, the contributor will put back in the repo the new version he just wrote (called *pushing*).  

First, in order to use github, you need to install gitBash which will allow you to push and pull (amongst other things). Go to 
[this website](https://git-scm.com/download/win) and download the version corresponding to your OS. Double click the file and 
press next until it starts downloading. When done hit finish. You should now be able to select *Git Bash Here* when right clicking in a folder
in the file explorer.

## Start Coding

The first time you'll want to code, you'll have to clone the repository on to your PC. To do this, browse to the location you want to store the
repo in the file explorer and right click and hit *Git Bash Here*. In the console type `git clone https://github.com/ERT-Vostok/AV-MainProgam.git`. 
At this point you may have to log in. If it denies permission, you're either using the wrong account either you really don't have a permission in which 
case you should contact Théo Houle (theo.houle@epfl.ch or ask on slack). When done, you'll have a folder with the name of the repo and inside it there 
should be a folder grayed out called *.git*, this is what makes the link between your local file and the project repo on github. You can now simply start
working inside the files. **See the how to use guide**

## How to use 

1. Before you start working, make sure you have the last version of the repo. For this, 
   press right click in the folder where you cloned the project (where the *.git* file is) and select *Git Bash Here*. Type in the console 
   `git pull`. This command might produce some *conflicts* in your code. To solve this, see the trouble shooting section.
   When done, you can start coding.
2. Write some code locally on your computer.
3. When you're done working, go to the file where you cloned the repo and righ click. Choose *Git Bash Here*.
4. In the console type : `git commit -am "Your Message"` where your message should briefly descibe what you did
   this command will prepare the work you did to be sent in the github repo and adds a little description.
5. Type `git push`. This might also produce some conflicts (See the trouble shooting section).
		
		
## Trouble Shooting

**Connection problems**: When cloning, pushing or pulling (or others), you might be asked to connect. If you can't access you're either using the wrong account
in which case you have to make sure you connect with the account linked to the repo. Otherwise, it most likely means that we made a mistake on our side and forgot
to give you acces. In this case, contact us (see contact us section)

**Conflicts**: When pushing or pulling, gitBash might warn you or even prevent you from continuing if you don't resolve the conflicts. This can also happen if you didn't pull before 
starting to code. Conflicts happen when you have modified the same bit of code as someone else and git can't figure out itself what to keep and what to discard. To solve this, go 
to the concerned file and look for the `<<<<<<<`, `>>>>>>>>` and the `Head` markers which points to the conflicts. Delete the parts you want to keep and make sure before trying 
to push again that you don't have any of these markers in your code. **Always push after you solved conflicts**.

*Something else*: You've came upon another issue I don't know yet. Don't give up, try first to find a solution by yourself on internet, if you can't contact me (see contacts below).
I repeat, **try to get it sorted yourself first**. But also, if you're not sure you're about to ruin the whole project, ask me.


## Contact us

If you have any problem **you can't solve by yourself** contact one of us either via mail or via slack. The latter is the best and quickest way to get a response. 

> Théo Houle (TL AV Vostok) : theo.houle@epfl.ch 

> Gaston Wolfart (TL AV Vostok) : gaston.wolfart@epfl.ch 


	
