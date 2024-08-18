from pyannote.audio import Pipeline
import torch
from dotenv import load_dotenv
import os

load_dotenv()
HUGGINGFACE_ACCESS_TOKEN = os.getenv("HUGGINGFACE_ACCESS_TOKEN")

pipeline = Pipeline.from_pretrained(
    "pyannote/speaker-diarization-3.1", use_auth_token=HUGGINGFACE_ACCESS_TOKEN
)
pipeline.to(torch.device("cuda"))


def get_diarization(filepath):
    diarization = pipeline(filepath)
    response = []
    for turn, _, speaker in diarization.itertracks(yield_label=True):
        response.append({"start": turn.start, "stop": turn.end, "speaker": speaker})
    return response
