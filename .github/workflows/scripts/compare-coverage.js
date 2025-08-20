#!/usr/bin/env node

import fs from 'fs';

function loadCoverage(filepath) {
  const content = fs.readFileSync(filepath, 'utf-8');
  return JSON.parse(content);
}

function extractFileCoverage(data) {
  const coverage = {};
  
  if (data.data && data.data.length > 0) {
    const files = data.data[0].files || [];
    for (const file of files) {
      // Remove nix store prefixes from filenames
      const filename = file.filename.replace(/^\/nix\/store\/[^/]+\//, '');
      coverage[filename] = {
        lines: file.summary.lines,
        functions: file.summary.functions,
        branches: file.summary.branches,
      };
    }
  }
  
  return coverage;
}

function formatPercentChange(oldVal, newVal) {
  const diff = newVal - oldVal;
  if (diff > 0) {
    return `+${diff.toFixed(2)}%`;
  } else {
    return `${diff.toFixed(2)}%`;
  }
}

function hasSignificantChange(baseFile, headFile, threshold = 0.01) {
  // Check if any metric changed by more than threshold percent
  const linesDiff = Math.abs((headFile?.lines?.percent || 0) - (baseFile?.lines?.percent || 0));
  const funcsDiff = Math.abs((headFile?.functions?.percent || 0) - (baseFile?.functions?.percent || 0));
  const branchesDiff = Math.abs((headFile?.branches?.percent || 0) - (baseFile?.branches?.percent || 0));
  
  return linesDiff > threshold || funcsDiff > threshold || branchesDiff > threshold;
}

function generateMarkdownReport(baseCov, headCov) {
  const report = [];
  report.push("## Coverage Report\n");
  
  const allFiles = new Set([...Object.keys(baseCov), ...Object.keys(headCov)]);
  
  if (allFiles.size === 0) {
    report.push("*No coverage data available.*");
    return report.join('\n');
  }
  
  // Filter to only files with coverage changes
  const changedFiles = Array.from(allFiles).filter(filename => {
    const baseFile = baseCov[filename];
    const headFile = headCov[filename];
    return hasSignificantChange(baseFile, headFile);
  }).sort();
  
  if (changedFiles.length === 0) {
    report.push("**No coverage changes detected**\n");
    report.push("*No files had significant coverage changes in this PR.*");
    return report.join('\n');
  }
  
  report.push(`### Files with Coverage Changes (${changedFiles.length})\n`);
  report.push("| File | Metric | Base | Head | Change |");
  report.push("|------|--------|------|------|--------|");
  
  const sortedFiles = changedFiles;
  
  for (const filename of sortedFiles) {
    const baseFile = baseCov[filename];
    const headFile = headCov[filename];
    
    // Lines coverage
    const baseLines = baseFile?.lines?.percent || 0;
    const headLines = headFile?.lines?.percent || 0;
    const linesChange = formatPercentChange(baseLines, headLines);
    
    // Functions coverage  
    const baseFuncs = baseFile?.functions?.percent || 0;
    const headFuncs = headFile?.functions?.percent || 0;
    const funcsChange = formatPercentChange(baseFuncs, headFuncs);
    
    // Branches coverage
    const baseBranches = baseFile?.branches?.percent || 0;
    const headBranches = headFile?.branches?.percent || 0;
    const branchesChange = formatPercentChange(baseBranches, headBranches);
    
    // Add rows for this file
    report.push(`| **${filename}** | Lines | ${baseLines.toFixed(1)}% | ${headLines.toFixed(1)}% | ${linesChange} |`);
    report.push(`| | Functions | ${baseFuncs.toFixed(1)}% | ${headFuncs.toFixed(1)}% | ${funcsChange} |`);
    report.push(`| | Branches | ${baseBranches.toFixed(1)}% | ${headBranches.toFixed(1)}% | ${branchesChange} |`);
  }

  return report.join('\n');
}

// Main execution
function main() {
  try {
    // Load coverage data
    const baseData = loadCoverage('base/coverage-base.json');
    const headData = loadCoverage('head/coverage-head.json');
    
    // Extract coverage metrics
    const baseCoverage = extractFileCoverage(baseData);
    const headCoverage = extractFileCoverage(headData);
    
    // Generate report
    const report = generateMarkdownReport(baseCoverage, headCoverage);
    
    // Save report
    fs.writeFileSync('coverage_report.md', report);
    console.log(report);
    
    process.exit(0);
  } catch (error) {
    process.exit(1);
  }
}

main();