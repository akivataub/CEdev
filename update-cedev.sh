#!/bin/bash
# Script to auto-upload files in ~/Documents/CEdev to GitHub
REPO_DIR="/home/akiva/Documents/CEdev"
LOG_FILE="/home/akiva/Documents/CEdev/update.log"

# Navigate to the repository
cd "$REPO_DIR" || {
    echo "$(date): Failed to change to $REPO_DIR" >> "$LOG_FILE"
    exit 1
}

# Stage and commit any unstaged changes
git add .
if ! git diff-index --quiet HEAD --; then
    git commit -m "Auto-commit unstaged changes: $(date)" && {
        echo "$(date): Committed unstaged changes" >> "$LOG_FILE"
    } || {
        echo "$(date): Commit of unstaged changes failed" >> "$LOG_FILE"
    }
fi

# Pull latest changes to avoid conflicts
git pull origin main --rebase || {
    echo "$(date): Pull failed" >> "$LOG_FILE"
    exit 1
}

# Add all files (in case new files were added after pull)
git add .

# Commit changes (only if there are changes)
if git diff-index --quiet HEAD --; then
    echo "$(date): No changes to commit" >> "$LOG_FILE"
else
    git commit -m "Auto-commit: $(date)" && {
        echo "$(date): Committed changes" >> "$LOG_FILE"
    } || {
        echo "$(date): Commit failed" >> "$LOG_FILE"
    }
fi

# Push to GitHub
git push origin main && {
    echo "$(date): Pushed to GitHub" >> "$LOG_FILE"
} || {
    echo "$(date): Push failed" >> "$LOG_FILE"
    exit 1
}
