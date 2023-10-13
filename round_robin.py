import random
import matplotlib.pyplot as plt

def round_robin(n_processos, quantum):
  processos = []
  tempos_de_espera = [0] * n_processos
  tempos_de_retorno = [0] * n_processos
  tempos_de_execucao = [0] * n_processos

  for i in range(n_processos):
    processos.append((i, random.randint(1, 20)))  # Simula burst time entre 1 e 20

  relogio = 0
  fila = list(processos)

  while fila:
    processo_atual = fila.pop(0)
    tempo_executado = min(processo_atual[1], quantum)
    
    tempos_de_execucao[processo_atual[0]] += tempo_executado
    relogio += tempo_executado

    for processo in fila:
      tempos_de_espera[processo[0]] += tempo_executado

    if processo_atual[1] > quantum:
      fila.append((processo_atual[0], processo_atual[1] - quantum))

  for i in range(n_processos):
    tempos_de_retorno[i] = tempos_de_execucao[i]

  return tempos_de_espera, tempos_de_retorno, tempos_de_execucao

def main():
  n_processos = 5
  quantums = [1, 2, 3, 4, 5]

  for quantum in quantums:
    tempos_de_espera, tempos_de_retorno, tempos_de_execucao = round_robin(n_processos, quantum)

    tempo_medio_de_espera = sum(tempos_de_espera) / n_processos
    tempo_medio_de_retorno = sum(tempos_de_retorno) / n_processos
    vazao = n_processos

    print(f"Quantum: {quantum}")
    print(f"Tempo Médio de Espera: {tempo_medio_de_espera:.2f}")
    print(f"Tempo Médio de Retorno: {tempo_medio_de_retorno:.2f}")
    print(f"Vazão: {vazao}")

    # Gera um gráfico do tempo de execução de cada processo
    plt.figure()
    plt.barh(range(n_processos), tempos_de_execucao)
    plt.xlabel("Tempo de Execução (unidades de tempo)")
    plt.ylabel("Processo")
    plt.title(f'Quantum: {quantum}')
    plt.show()

if __name__ == "__main__":
  main()
