#!/bin/bash
# Script to auto-upload files in ~/Documents/CEdev to GitHub
REPO_DIR="/home/akiva/Documents/CEdev"
LOG_FILE="/home/akiva/update-cedev.log"

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

# Stage and commit any new changes (e.g., from merge)
git add .
if ! git diff-index --quiet HEAD --; then
    git commit -m "Auto-commit after pull: $(date)" && {
        echo "$(date): Committed changes after pull" >> "$LOG_FILE"
    } || {
        echo "$(date): Commit after pull failed" >> "$LOG_FILE"
    }
else
    echo "$(date): No changes to commit after pull" >> "$LOG_FILE"
fi

# Push to GitHub
git push origin main && {
    echo "$(date): Pushed to GitHub" >> "$LOG_FILE"
} || {
    echo "$(date): Push failed" >> "$LOG_FILE"
    exit 1
}
