Github
======
The github shell script is a wrapper for all the other functions in the folder meant to make working with github easier from the command line by linking into the provided github API and automating routine actions


Upload Key
----------
This script takes advantage of the github API to allow a user to run a simple script, have the computer generate a new ssh keypair and then upload the pubkey to the github account. Simply run `addKey.sh` in order to upload your pub key, if you do not have an ssh keypair you will be prompted to generate one. Fill in your github username and password along with the computer name you want github to associate this key with.

New Repo
--------
Prompts the user for all the settings they can choose for creating a new repo
allowing for certain settings to be left blank and have default values fill them. After the repo is created it is automatically added to remotes and pushed if your current directory is a git repo.