import matplotlib.pyplot as plt
import csv


numero_atomico = []
neutrones_reales = []
neutrones_predichos = []

with open('datos.csv', 'r') as csvfile:
    csvreader = csv.reader(csvfile)
    next(csvreader) 
    for row in csvreader:
        numero_atomico.append(int(row[0]))
        neutrones_reales.append(int(row[1]))
        neutrones_predichos.append(float(row[2]))

plt.figure(figsize=(10, 6))
plt.scatter(numero_atomico, neutrones_reales, color='blue', label='Neutrones Reales')
plt.plot(numero_atomico, neutrones_predichos, color='red', label='Neutrones Predichos')
plt.xlabel('Número Atómico')
plt.ylabel('Neutrones')
plt.title('Relación entre Número Atómico y Neutrones')
plt.legend()
plt.grid(True)
plt.show()
