echo "Username:Password:Last Change:Minimum days:Maximum Days:Days to Warn:Days to Expire:Account Disabled" | cat - /etc/shadow | sed "s/::/: :/g;" | column -s : -t


### What do I want this program to be able to do? Show me all the users and groups inside of the system and who is a part of what group?
### What will that allow me to do? I dont know what permissions are requeired in order to run any applicaitons


### I want to be able to see a list of users and groups and interact with that list to add and remove people fro mthose groups
### ok this sounds like a program that should exists now. I like this idea