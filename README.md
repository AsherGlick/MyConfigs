MyConfigs
=========

My personal repository for my configuration files and simple scripts to allow me
to quickly do routine tasks, or automate seldom used tasks to prevent having to
look up how to do them again, tasks like initializing settings on a new install
of an operating system.

How to use it
-------------
If you want to use it yourself it is quite simple. First you will want to fork
this repository so that you have your own place to upload your configurations
to. After you have your own repository open up the scriptconfig file

    gedit scripconfig

which is a JSON file containing all the entries on which files to back up

    {
		"bashrc":"~/.bashrc",
		"zshrc":"~/.zshrc",
		"gitconfig":"~/.gitconfig",
		"Name_Of_Configuation":"Path/To/Configuration",
	}


`packscripts.py` handles all of the configuration needs allowing automatic
backup and restoring of files as well as the ability to create a symlink for a
file preventing it from ever being out of sync with this repository

![Running Packsrips.py])(http://i.imgur.com/fCPQ5En.png)