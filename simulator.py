
import numpy as np

LINE_FREQUENCY = 50


class Phase:

    def __init__(self):
        self.VRMS = 220
        self.IRMS = 10
        self.PHI = 0

        self.P_REAL = self.VRMS * self.IRMS * np.cos(self.PHI)

        self.FS = 1e3
        self.N_PERIODS = 2

    def generate(self):
        t = np.arange(0, self.N_PERIODS*1/LINE_FREQUENCY, 1/self.FS)
        v = np.sqrt(2) * self.VRMS * np.sin(2*np.pi*LINE_FREQUENCY * t)
        i = np.sqrt(2) * self.IRMS * np.sin(2*np.pi*LINE_FREQUENCY * t - self.PHI)

        self.t, self.v, self.i = t, v, i

    def power_error_percentage(self, measured_power):
        return np.abs(measured_power - self.P_REAL) / self.P_REAL * 100


def error_by_FS():
    for FS in [100, 110, 200, 500, 1000, 10000]:
        phase = Phase()
        phase.FS = FS
        phase.generate()
        measured_power = np.mean(phase.v * phase.i)
        print('{}\t:{}'.format(FS, phase.power_error_percentage(measured_power)))


def save_signals():
    phase = Phase()
    phase.generate()

    def write_signal(fout, signal, label):
        fout.write('float {}[] = {{ '.format(label))
        fout.write(',\n              '.join(map(lambda v: '{:.1f}'.format(v), signal)))
        fout.write('\n};')

    with open('mock_signals.h', 'w') as fout:
        fout.write('// FS = {:.0f}\n\n'.format(phase.FS))
        fout.write('#define MOCK_N {:d}\n\n'.format(len(phase.v)))
        write_signal(fout, phase.v, 'V')
        fout.write('\n\n')
        write_signal(fout, phase.i, 'I')
save_signals()
