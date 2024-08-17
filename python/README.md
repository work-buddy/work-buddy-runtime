This `/python` folder is for running Huggingface-like AI models locally as micro-services, fetched on localhost by the main WorkBuddy C++ runtime. Ideally we want everything in C++ natively for realtime but can't remake every AI model, sadly the AI space is 90% Python.

## Setup

Python version = `3.11`

```bash
python3 -m venv workbuddy
source workbuddy/bin/activate
pip install -r requirements.txt
```

Environment Variables come from the workspace root `.env` outside of `/python`. `.env` is shared by C++ and Python.

## Modules

### pyannote-audio

https://github.com/pyannote/pyannote-audio

This model performs speech segmentation and diarization that can filter groups of speakers into categories. Useful for speech recognition commands and transcriptions.

Configure `HUGGINGFACE_ACCESS_TOKEN` in the `.env` file.

Accept the gated access user conditions on HuggingFace:

https://huggingface.co/pyannote/speaker-diarization-3.1
https://huggingface.co/pyannote/segmentation-3.0

```bash
python3 pyannote/main.py
```
