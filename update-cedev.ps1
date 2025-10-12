param(
    [switch]$Silent = $false
)

$REPO_DIR = "C:\Users\Akiva\Documents\CEdev"
$LOG_FILE = "C:\Users\Akiva\Documents\CEdev\update-cedev.log"

function Write-Log {
    param([string]$Message)
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $logEntry = "[$timestamp] $Message"
    Add-Content -Path $LOG_FILE -Value $logEntry
    if (-not $Silent) { Write-Host $logEntry }
}

try {
    Set-Location $REPO_DIR
    Write-Log "Started check in $REPO_DIR"
} catch {
    Write-Log "Failed to change to ${REPO_DIR}: $($_.Exception.Message)"
    exit 1
}

# Check for any changes (unstaged or uncommitted)
$status_output = git status --porcelain 2>$null
if ($status_output -eq "") {
    Write-Log "No changes detected - skipping upload"
    exit 0  # Success, but no action
}

Write-Log "Changes detected - staging and committing"

# Stage all changes
git add . 2>$null
if ($LASTEXITCODE -ne 0) {
    Write-Log "git add failed"
    exit 1
}

# Commit
git commit -m "Auto-upload changes: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" 2>$null
if ($LASTEXITCODE -ne 0) {
    Write-Log "Commit failed"
    exit 1
}

Write-Log "Committed changes"

# Pull first (merge strategy for safety)
git pull origin master 2>$null
if ($LASTEXITCODE -ne 0) {
    Write-Log "Pull failed"
    exit 1
}

# Re-stage and commit any merge conflicts/resolutions (if needed)
git add . 2>$null
$status_after_pull = git status --porcelain 2>$null
if ($status_after_pull -ne "") {
    git commit -m "Auto-resolve after pull: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" 2>$null
    if ($LASTEXITCODE -ne 0) {
        Write-Log "Post-pull commit failed"
        exit 1
    }
    Write-Log "Committed post-pull changes"
}

# Push
git push origin master 2>$null
if ($LASTEXITCODE -eq 0) {
    Write-Log "Pushed successfully"
} else {
    Write-Log "Push failed"
    exit 1
}

Write-Log "Auto-upload completed"