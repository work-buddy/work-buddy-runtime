# main.py

# Import necessary libraries
from pyannote.audio import Pipeline
import torch
from dotenv import load_dotenv
import os

load_dotenv()

HUGGINGFACE_ACCESS_TOKEN= os.getenv("HUGGINGFACE_ACCESS_TOKEN")

pipeline = Pipeline.from_pretrained(
    "pyannote/speaker-diarization-3.1",
    use_auth_token=HUGGINGFACE_ACCESS_TOKEN
)

pipeline.to(torch.device("cuda"))

diarization = pipeline("harry-potter.wav")

for turn, _, speaker in diarization.itertracks(yield_label=True):
    print(f"start={turn.start:.1f}s stop={turn.end:.1f}s speaker_{speaker}")