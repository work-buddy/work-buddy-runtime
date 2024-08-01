from ultralytics import YOLO

# Load a model
model = YOLO("models/pre-trained/yolov8x.pt")

# Export the model
model.export(format="onnx")