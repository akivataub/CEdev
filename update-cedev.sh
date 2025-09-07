#!/bin/bash
REPO_DIR="/home/akiva/Documents/CEdev"
LOG_FILE="/home/akiva/update-cedev.log"
cd "$REPO_DIR" || { echo "$(date): Failed to change to $REPO_DIR" >> "$LOG_FILE"; exit 1; }
git add .
if ! git diff-index --quiet HEAD --; then
    git commit -m "Auto-commit unstaged changes: $(date)" && { echo "$(date): Committed unstaged changes" >> "$LOG_FILE"; } || { echo "$(date): Commit of unstaged changes failed" >> "$LOG_FILE"; }
fi
git pull origin master --rebase || { echo "$(date): Pull failed" >> "$LOG_FILE"; exit 1; }
git add .
if ! git diff-index --quiet HEAD --; then
    git commit -m "Auto-commit after pull: $(date)" && { echo "$(date): Committed changes after pull" >> "$LOG_FILE"; } || { echo "$(date): Commit after pull failed" >> "$LOG_FILE"; }
else
    echo "$(date): No changes to commit after pull" >> "$LOG_FILE"
fi
git push origin master && { echo "$(date): Pushed to GitHub" >> "$LOG_FILE"; } || { echo "$(date): Push failed" >> "$LOG_FILE"; exit 1; }
