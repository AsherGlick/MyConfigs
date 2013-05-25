PRIVATE_REPO_DEFAULT='false'
ISSUE_PAGE_DEFAULT='true'
WIKI_PAGE_DEFAULT='true'
DOWNLOADS_DEFAULT='true'


PRIVATE="X"
ISSUE="X"
WIKI="X"
DOWNLOADS="X"
# Get input for all the settings
read -p "Repo Name: " NAME
read -p "Repo Description: " DESCRIPTION
read -p "Webpage: " WEBPAGE


#################
while [ "$PRIVATE" != "true" -a "$PRIVATE" != "false" -a "$PRIVATE" != "" ]
do
	read -p "Private Repo? (Default $PRIVATE_REPO_DEFAULT) " PRIVATE
	if [ "$PRIVATE" != "true" -a "$PRIVATE" != "false" -a "$PRIVATE" != "" ]
	then
		echo "    Values can only be (true / false)"
	fi
done
if [ "$PRIVATE" = "" ]
then
	PRIVATE="$PRIVATE_REPO_DEFAULT"
fi

##################
while [ "$ISSUE" != "true" -a "$ISSUE" != "false" -a "$ISSUE" != "" ]
do
	read -p "Issue Page? (Default $ISSUE_PAGE_DEFAULT) " ISSUE 
	if [ "$ISSUE" != "true" -a "$ISSUE" != "false" -a "$ISSUE" != "" ]
	then
		echo "    Values can only be (true / false)"
	fi
done
if [ "$ISSUE" = "" ]
then
	ISSUE="$ISSUE_PAGE_DEFAULT"
fi

###############
while [ "$WIKI" != "true" -a "$WIKI" != "false" -a "$WIKI" != "" ]
do
	read -p "Wiki? (Default $WIKI_PAGE_DEFAULT) " WIKI
	if [ "$WIKI" != "true" -a "$WIKI" != "false" -a "$WIKI" != "" ]
	then
		echo "    Values can only be (true / false)"
	fi
done
if [ "$WIKI" = "" ]
then
	WIKI="$WIKI_PAGE_DEFAULT"
fi




#################
while [ "$DOWNLOADS" != "true" -a "$DOWNLOADS" != "false" -a "$DOWNLOADS" != "" ]
do
	read -p "Downloads? (Default $DOWNLOADS_DEFAULT) " DOWNLOADS
	if [ "$DOWNLOADS" != "true" -a "$DOWNLOADS" != "false" -a "$DOWNLOADS" != "" ]
	then
		echo "    Values can only be (true / false)"
	fi
done
if [ "$DOWNLOADS" = "" ]
then
	DOWNLOADS="$DOWNLOADS_DEFAULT"
fi

read -p "Username: " USERNAME


curl -i -u "$USERNAME" -d "{ \"name\": \"$NAME\", \"description\": \"$DESCRIPTION\", \"homepage\": \"$WEBPAGE\", \"private\": $PRIVATE, \"has_issues\": $ISSUE, \"has_wiki\": $WIKI, \"has_downloads\": $DOWNLOADS }" https://api.github.com/user/repos