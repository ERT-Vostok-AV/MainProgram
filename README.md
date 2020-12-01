# Vostok AV GitHub Repository
This is the repo for the arduino code of the avionic

## Beginner's guide: 

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
   press shift + right click at the same time and select `git bash here` and type git pull
   in the console and wait for it to finish.
2. Write some code locally on your computer.
3. When you're done working, got to the file where you cloned the repo and press shift+rightClick
   and go to open Bash here.
4. In the console type : `git commit -am "Your Message"` where your message should briefly descibe what you did
   this command will prepare the work you did to be sent in the repo and add a little description
5. Type git push 
		
	
