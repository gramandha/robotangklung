import numpy as np
import cv2
import time
import torch
import pathlib
import serial
from pathlib import Path
pathlib.PosixPath = pathlib.WindowsPath

# Inisialisasi komunikasi serial dengan Arduino
try:
    arduino = serial.Serial('COM7', 2000000, timeout=1)
except serial.SerialException:
    print("Gagal membuka port serial. Pastikan Arduino terhubung.")
    exit()

# Muat model YOLOv5
model = torch.hub.load(
    'C:\\wahyu\\semester 7\\angklung\\epoch 200\\yolov5 (17)\\kaggle\\working\\yolov5',
    'custom',
    path='C:\\\wahyu\\semester 7\\angklung\\epoch 200\\yolov5 (17)\\kaggle\\working\\yolov5\\runs\\train\\exp\\weights\\best.pt',
    source='local',)

# Inisialisasi kamera
camera1 = cv2.VideoCapture(0)
camera1.set(3, 720)
camera1.set(4, 720)

if not camera1.isOpened():
    print("Gagal membuka kamera.")
    exit()
frame_count = 0
start_time = time.time()

while True:
    ret0, frame = camera1.read()
    if not ret0:
        print("Gagal membaca frame dari kamera")
        break
    frame = cv2.flip(frame, 1)

    height, width, _ = frame.shape

    # Ukuran dan posisi kotak merah
    box_width = 200
    box_height = 210

    # Kotak Kiri (untuk semua not kecuali do-)
    left_x1 = (width // 2) - (box_width // 2) - (box_width + 20)  # Geser ke kiri
    left_y1 = (height // 2) - (box_height // 2)
    left_x2 = left_x1 + box_width
    left_y2 = left_y1 + box_height

    # Kotak Kanan (hanya untuk do-)
    right_x1 = (width // 2) + (box_width // 2) + 20  # Geser ke kanan
    right_y1 = left_y1
    right_x2 = right_x1 + box_width
    right_y2 = right_y1 + box_height

    # Gambar kotak merah
    box_color = (0, 0, 255)  # Merah
    box_thickness = 2
    cv2.rectangle(frame, (left_x1, left_y1), (left_x2, left_y2), box_color, box_thickness)  # Kiri
    cv2.rectangle(frame, (right_x1, right_y1), (right_x2, right_y2), box_color, box_thickness)  # Kanan

    # Deteksi objek dengan YOLOv5
    detections = model(frame[..., ::-1])
    results = detections.pandas().xyxy[0].to_dict(orient="records")

    not_mapping = {
        "do": b'A', "re": b'B', "mi": b'C', "fa": b'D',
        "sol": b'E', "la": b'F', "si": b'G'
    }

    for result in results:
        cs = result['name']
        x1 = int(result['xmin'])
        y1 = int(result['ymin'])
        x2 = int(result['xmax'])
        y2 = int(result['ymax'])

        # Gambar bounding box objek
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
        cv2.putText(frame, f"{cs} ({result['confidence']:.2f})", (x1, y1 - 10), cv2.FONT_HERSHEY_PLAIN, 2, (255, 255, 0), 2)

        # Cek apakah objek berada dalam salah satu kotak merah
        if left_x1 < x1 < left_x2 and left_y1 < y1 < left_y2 and left_x1 < x2 < left_x2 and left_y1 < y2 < left_y2:
            if cs == "do":
               arduino.write(b'H')
               print("do- (kotak kanan)")

        elif right_x1 < x1 < right_x2 and right_y1 < y1 < right_y2 and right_x1 < x2 < right_x2 and right_y1 < y2 < right_y2:
            if cs in not_mapping: 
                arduino.write(not_mapping[cs])
                print(f"{cs} (kotak kiri)")
    frame_count += 1
    elapsed_time = time.time() - start_time
    if elapsed_time > 0:
        avg_fps = frame_count / elapsed_time
    cv2.putText(frame, f"FPS: {avg_fps:.2f}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 255), 2)


    # Cek data dari Arduino
    if arduino.in_waiting > 0:
        try:
            serial_data = arduino.readline().decode('utf-8').rstrip()
            print(f"Data dari Arduino: {serial_data}")
        except Exception as e:
            print(f"Error membaca data serial: {e}")

    cv2.imshow('Camera 1', frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

camera1.release()
arduino.close()
cv2.destroyAllWindows()
