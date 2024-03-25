import matplotlib.pyplot as plt

v_primary = range(0, 425)
v_secundary = [x / 484.09877 for x in v_primary]

plt.figure(figsize=(10, 6))
plt.plot(v_primary, v_secundary, color='blue')
plt.xlabel('Tensão entre Fase e Neutro nos Enrolamentos Primários (V)')
plt.ylabel('Tensão entre Fase e Neutro nos Enrolamentos Secundários (V)')
plt.title('Relação de Transformação da Tensão entre Fase e Neutro')
plt.grid(True)
plt.show()

raise_of_three = 3 ** 0.5
v_primary = [x * raise_of_three for x in v_primary]
v_secundary = [x / 838.48367 for x in v_primary]

plt.figure(figsize=(10, 6))
plt.plot(v_primary, v_secundary, color='red')
plt.xlabel('Tensão entre Fase e Fase nos Enrolamentos Primários (V)')
plt.ylabel('Tensão entre Fase e Fase nos Enrolamentos Secundários (V)')
plt.title('Relação de Transformação da Tensão entre Fase e Fase')
plt.grid(True)
#plt.show()