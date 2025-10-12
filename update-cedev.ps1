# PowerShell script for updating CEdev repo on Windows
# Save as update-cedev.ps1 and run with: powershell -ExecutionPolicy Bypass -File update-cedev.ps1

$REPO_DIR = "C:\Users\Akiva\Documents\CEdev"
$LOG_FILE = "C:\Users\Akiva\Documents\CEdev\update-cedev.log"

try {
    Set-Location $REPO_DIR
    Add-Content -Path $LOG_FILE -Value "$(Get-Date): Changed to $REPO_DIR"
} catch {
    Add-Content -Path $LOG_FILE -Value "$(Get-Date): Failed to change to $REPO_DIR"
    exit 1
}

git add .

# Check if there are unstaged changes
if (-not (git diff-index --quiet HEAD --)) {
    $commit_result = git commit -m "Auto-commit unstaged changes: $(Get-Date)"
    if ($LASTEXITCODE -eq 0) {
        Add-Content -Path $LOG_FILE -Value "$(Get-Date): Committed unstaged changes"
    } else {
        Add-Content -Path $LOG_FILE -Value "$(Get-Date): Commit of unstaged changes failed"
    }
}

$pull_result = git pull origin master --rebase
if ($LASTEXITCODE -ne 0) {
    Add-Content -Path $LOG_FILE -Value "$(Get-Date): Pull failed"
    exit 1
}

git add .

# Check if there are changes after pull
if (-not (git diff-index --quiet HEAD --)) {
    $commit_result = git commit -m "Auto-commit after pull: $(Get-Date)"
    if ($LASTEXITCODE -eq 0) {
        Add-Content -Path $LOG_FILE -Value "$(Get-Date): Committed changes after pull"
    } else {
        Add-Content -Path $LOG_FILE -Value "$(Get-Date): Commit after pull failed"
    }
} else {
    Add-Content -Path $LOG_FILE -Value "$(Get-Date): No changes to commit after pull"
}

$push_result = git push origin master
if ($LASTEXITCODE -eq 0) {
    Add-Content -Path $LOG_FILE -Value "$(Get-Date): Pushed to GitHub"
} else {
    Add-Content -Path $LOG_FILE -Value "$(Get-Date): Push failed"
    exit 1
}