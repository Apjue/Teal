#!/bin/bash

if [ -z "$2" ]; then
  echo -e "WARNING!!\nYou need to pass the WEBHOOK_URL environment variable as the second argument to this script.\nFor details & guide, visit: https://github.com/k3rn31p4nic/travis-ci-discord-webhook" && exit
fi

echo -e "[Webhook]: Sending webhook to Discord...\\n";

case $1 in
  "success" )
    EMBED_COLOR=3066993
    STATUS_MESSAGE="passed"
    ;;

  "failure" )
    EMBED_COLOR=15158332
    STATUS_MESSAGE="failed"
    ;;

  * )
    EMBED_COLOR=0
    STATUS_MESSAGE=": Status Unknown"
    ;;
esac

AVATAR="https://travis-ci.org/images/logos/TravisCI-Mascot-1.png"

AUTHOR_NAME="$(git log -1 "$TRAVIS_COMMIT" --pretty="%aN")"
COMMITTER_NAME="$(git log -1 "$TRAVIS_COMMIT" --pretty="%cN")"
COMMIT_SUBJECT="$(git log -1 "$TRAVIS_COMMIT" --pretty="%s")"
COMMIT_MESSAGE="$(git log -1 "$TRAVIS_COMMIT" --pretty="%b")"

if [ "$AUTHOR_NAME" == "$COMMITTER_NAME" ]; then
  CREDITS="$AUTHOR_NAME authored & committed"
else
  CREDITS="$AUTHOR_NAME authored & $COMMITTER_NAME committed"
fi

TIMESTAMP=$(date --utc +%FT%TZ) # "${COMMIT_MESSAGE//$'\n'/ }"

if [ "$STATUS_MESSAGE" == "passed" ]; then
  WEBHOOK_DATA='{
    "username": "Travis",
    "avatar_url": "https://travis-ci.org/images/logos/TravisCI-Mascot-1.png",
    "embeds": [ {
      "color": '$EMBED_COLOR',
      "author": {
        "name": "'"$TRAVIS_REPO_SLUG"':'"$TRAVIS_BRANCH"'",
        "url": "https://github.com/'"$TRAVIS_REPO_SLUG"'/tree/'"$TRAVIS_BRANCH"'",
        "icon_url": "'$AVATAR'"
      },
      "title": "'"$COMMIT_SUBJECT"'",
      "url": "'"https://github.com/$TRAVIS_REPO_SLUG/commit/$TRAVIS_COMMIT"'",
      "description": "Build [#'"$TRAVIS_JOB_NUMBER"'](https://travis-ci.org/'"$TRAVIS_REPO_SLUG"'/builds/'"$TRAVIS_BUILD_ID"') '"$STATUS_MESSAGE"' ('"${CONFIG}"' '"${PLATFORM}"') - '"$CREDITS"' '"[\`${TRAVIS_COMMIT:0:7}\`](https://github.com/$TRAVIS_REPO_SLUG/commit/$TRAVIS_COMMIT)"' - [Artefact link](https://github.com/'"$DEPLOY_REPO_SLUG"'/tree/'"$DEPLOY_BRANCH"')"
    } ]
  }'
else
  WEBHOOK_DATA='{
    "username": "Travis",
    "avatar_url": "https://travis-ci.org/images/logos/TravisCI-Mascot-1.png",
    "embeds": [ {
      "color": '$EMBED_COLOR',
      "author": {
        "name": "'"$TRAVIS_REPO_SLUG"':'"$TRAVIS_BRANCH"'",
        "url": "https://github.com/'"$TRAVIS_REPO_SLUG"'/tree/'"$TRAVIS_BRANCH"'",
        "icon_url": "'$AVATAR'"
      },
      "title": "'"$COMMIT_SUBJECT"'",
      "url": "'"https://github.com/$TRAVIS_REPO_SLUG/commit/$TRAVIS_COMMIT"'",
      "description": "Build [#'"$TRAVIS_JOB_NUMBER"'](https://travis-ci.org/'"$TRAVIS_REPO_SLUG"'/builds/'"$TRAVIS_BUILD_ID"') '"$STATUS_MESSAGE"' ('"${CONFIG}"' '"${PLATFORM}"') - '"$CREDITS"' '"[\`${TRAVIS_COMMIT:0:7}\`](https://github.com/$TRAVIS_REPO_SLUG/commit/$TRAVIS_COMMIT)"'"
    } ]
  }'
fi

(curl --fail --progress-bar -A "TravisCI-Webhook" -H Content-Type:application/json -H X-Author:k3rn31p4nic#8383 -d "$WEBHOOK_DATA" "$2" \
  && echo -e "\\n[Webhook]: Successfully sent the webhook.") || echo -e "\\n[Webhook]: Unable to send webhook."
