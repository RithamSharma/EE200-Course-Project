# MLLM Vision Module - BDD Specifications

## Feature: Vision Screen Analysis

### Scenario: Full Screen Capture and Analysis

**Given** NovaShell is running with valid Gemini API authentication  
**When** the user types `see` without arguments  
**Then** the system captures the entire screen  
**And** opens an interactive TUI showing the captured image  
**And** prompts the user to select analysis mode  
**And** sends the image to Gemini for analysis  
**And** displays the AI response in the terminal

### Scenario: Region Selection with TUI

**Given** NovaShell is running with valid Gemini API authentication  
**When** the user types `see` and enters capture mode  
**Then** the system displays captured screen in TUI  
**And** allows arrow keys to select a region via overlay  
**And** shows coordinates and dimensions in real-time  
**And** pressing Enter confirms selection  
**And** only the selected region is sent for analysis

### Scenario: Analysis Mode Selection

**Given** an image region is selected in the TUI  
**When** the selection is confirmed  
**Then** the system presents analysis mode menu:
- Screen Understanding
- OCR Text Extraction
- Accessibility Audit
- Full Analysis
**And** each mode sends appropriate system prompt to Gemini
**And** displays formatted results

### Scenario: Image File Analysis

**Given** NovaShell is running with valid Gemini API authentication  
**When** the user types `see /path/to/image.png`  
**Then** the system validates the file exists and is an image  
**And** loads and processes the image  
**And** proceeds to analysis mode selection
**And** sends the image to Gemini for analysis

### Scenario: Invalid Image File Handling

**Given** NovaShell is running with valid Gemini API authentication  
**When** the user types `see /path/to/invalid.file`  
**Then** the system displays error: "Invalid image file or unsupported format"
**And** lists supported formats: PNG, JPEG, WebP, BMP
**And** returns to shell prompt

### Scenario: No Authentication Handling

**Given** NovaShell is running without valid Gemini API authentication  
**When** the user types `see`  
**Then** the system displays: "Vision requires Gemini API authentication. Use 'login' or 'set-key' to configure."
**And** returns to shell prompt

### Scenario: Capture Failure Handling

**Given** screen capture is initiated  
**When** the platform capture fails (e.g., permissions denied)  
**Then** the system displays platform-specific error message  
**And** suggests remediation steps (e.g., "Grant screen capture permission")
**And** returns to shell prompt

## Feature: Platform-Specific Screen Capture

### Scenario: X11 Screen Capture

**Given** Linux with X11 session  
**When** `see` command is executed  
**Then** use xwd/xlib to capture root window  
**And** convert to PNG format  
**And** return image data for processing

### Scenario: Wayland Screen Capture

**Given** Linux with Wayland compositor  
**When** `see` command is executed  
**Then** use screencopy protocol via libwayland  
**And** convert to PNG format  
**And** fallback to gnome-screenshot if protocol unavailable

### Scenario: Hyprland Screen Capture

**Given** Linux with Hyprland compositor  
**When** `see` command is executed  
**Then** use Hyprland's internal screenshot API  
**And** convert to PNG format  
**And** use grim as fallback

### Scenario: Windows Screen Capture

**Given** Windows 10/11 (Win32/Win64)  
**When** `see` command is executed  
**Then** use GDI+ or DirectX for screen capture  
**And** convert to PNG format  
**And** handle multi-monitor setups

### Scenario: macOS Screen Capture

**Given** macOS  
**When** `see` command is executed  
**Then** use CGDisplayCapture/CoreGraphics  
**And** convert to PNG format  
**And** handle Retina display scaling

## Feature: Image Processing Pipeline

### Scenario: Base64 Encoding

**Given** PNG image data  
**When** processing for API transmission  
**Then** encode to base64 string  
**And** ensure no data corruption during encoding  
**And** include proper MIME type in request

### Scenario: Image Size Optimization

**Given** large image (>4MB after base64)  
**When** processing for API transmission  
**Then** resize image to maximum 1920x1080 while preserving aspect ratio  
**And** compress to reduce file size  
**And** show progress indicator during processing

### Scenario: Format Conversion

**Given** screenshot in native format (XWD, BMP, etc.)  
**When** preparing for API transmission  
**Then** convert to PNG format  
**And** strip unnecessary metadata  
**And** ensure color accuracy

## Feature: Analysis Modes

### Scenario: Screen Understanding Analysis

**Given** image is selected  
**When** user selects "Screen Understanding" mode  
**Then** send prompt: "Describe what's visible in this image in detail..."  
**And** request comprehensive description of UI elements, text, layout

### Scenario: OCR Text Extraction

**Given** image is selected  
**When** user selects "OCR Text Extraction" mode  
**Then** send prompt: "Extract all visible text from this image..."  
**And** format output as structured text with positions

### Scenario: Accessibility Audit

**Given** image is selected  
**When** user selects "Accessibility Audit" mode  
**Then** send prompt: "Analyze this UI for accessibility issues..."  
**And** identify contrast issues, small text, missing labels

### Scenario: Full Analysis

**Given** image is selected  
**When** user selects "Full Analysis" mode  
**Then** send prompt combining all analysis types  
**And** include screen understanding, OCR, accessibility, and recommendations

## Feature: Error Handling and Edge Cases

### Scenario: API Rate Limit

**Given** Gemini API rate limit is hit  
**When** vision analysis is attempted  
**Then** display friendly message with retry suggestion  
**And** provide countdown if applicable

### Scenario: Network Failure

**Given** network connectivity is lost during analysis  
**When** vision command is executing  
**Then** display connection error with troubleshooting steps  
**And** preserve captured image for retry

### Scenario: Empty Screen Capture

**Given** screen capture returns empty/corrupt data  
**When** image processing begins  
**Then** detect invalid image data  
**And** display error: "Capture failed - try again or check permissions"

### Scenario: API Timeout Handling

**Given** vision analysis is in progress  
**When** the API response takes longer than 30 seconds  
**Then** display timeout error message  
**And** preserve captured image for manual retry  
**And** suggest checking network connection

### Scenario: Auth Token Expiry

**Given** NovaShell has valid authentication  
**When** auth token expires mid-session  
**Then** detect 401 response from API  
**And** display: "Session expired. Please run 'login' or 'set-key' to re-authenticate."
**And** return to shell prompt without crashing

### Scenario: Large Image Rejection

**Given** the user provides an image file larger than 8MB  
**When** the image is being processed  
**Then** calculate estimated base64 size (file × 1.37)  
**And** if exceeds 5MB, resize to maximum 1920x1080 before encoding  
**And** show progress indicator during resize  
**And** proceed to analysis with optimized image