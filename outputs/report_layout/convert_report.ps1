$ErrorActionPreference = 'Stop'
$sourcePath = 'C:\Users\李\Desktop\杂项\AI-course\人工智能概论02_李培煜_2025141460032_第一次作业.doc'
$outputDir = 'D:\大二学年代码\outputs\report_layout'
$app = $null
foreach ($programId in @('Word.Application', 'KWPS.Application', 'WPS.Application')) {
    try {
        $app = New-Object -ComObject $programId
        Write-Output "Using $programId"
        break
    } catch { Write-Output "$programId unavailable" }
}
if ($null -eq $app) { throw 'No supported Word or WPS automation interface is installed.' }
$doc = $null
try {
    $app.Visible = $false
    $app.DisplayAlerts = 0
    $formattedPath = Join-Path $outputDir '人工智能概论_第一次作业_排版优化版.docx'
    if (Test-Path -LiteralPath $formattedPath) {
        $doc = $app.Documents.Open($formattedPath, $false, $true)
        $doc.ExportAsFixedFormat((Join-Path $outputDir 'formatted.pdf'), 17)
    } else {
        $doc = $app.Documents.Open($sourcePath, $false, $true)
        $doc.SaveAs2((Join-Path $outputDir 'source.docx'), 16)
        $doc.ExportAsFixedFormat((Join-Path $outputDir 'source.pdf'), 17)
    }
    Write-Output "Pages: $($doc.ComputeStatistics(2))"
} finally {
    if ($null -ne $doc) { $doc.Close(0) }
    $app.Quit()
}
