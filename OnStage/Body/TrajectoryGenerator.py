import numpy as np
import math
import matplotlib.pyplot as plt
np.set_printoptions(precision=3, suppress=True, linewidth=200)

# Global variables
global Alphas
global VecPos    
global VecVel
global VecAccel
global VecJerk
global Psi
global c

# Robot and trajectory parameters
V = 200  # mm/s
Tr = 360  # mm, Distance between wheels
R = 87.5  # mm, Radius of the wheel

# Waypoints in mm
waypoints = np.transpose(np.array([[0, 0],
                                   [1000, 1000]]))
d = waypoints[:,0:-1] - waypoints[:,1:]
Ts = np.sqrt(np.power(d[0,:],2) + np.power(d[1,:],2))/V  # Time for each segment
Si = np.append([0], np.cumsum(Ts))  # Cumulative sum of times
Tang = 1  # Tangent constraint flag
(_, nwp) = np.shape(waypoints)
n = nwp - 1

# Time vector
dT = 0.002
VecT = np.arange(0, Si[-1], dT)

# Polynomial trajectory calculation
if n == 1:       
    T = Ts[0]
    A1 = np.array([[1, 0, 0, 0, 0, 0],
                   [1, 1*T, 1*T**2, 1*T**3, 1*T**4, 1*T**5]])  # Conditions at start and end

    A2y = np.array([[0, 1, 0, 0, 0, 0],  # Velocity at start
                    [0, 1, 2*T, 3*T**2, 4*T**3, 5*T**4],  # Velocity at end
                    [0, 0, 2, 0, 0, 0],  # Acceleration at start
                    [0, 0, 2, 6*T, 12*T**2, 20*T**3]])  # Acceleration at end

    A2x = np.array([[0, 1, 0, 0, 0, 0],  # Velocity at start
                    [0, 1, 2*T, 3*T**2, 4*T**3, 5*T**4],  # Velocity at end
                    [0, 0, 2, 6*T, 12*T**2, 20*T**3],  # Acceleration at end
                    [0, 0, 0, 6, 0, 0]])  # Jerk at start or end

    Ax = np.append(A1, A2x, axis=0)
    Ay = np.append(A1, A2y, axis=0)
    bx = np.array([waypoints[0,0], waypoints[0,1], 0, 0, 0, 0])
    by = np.array([waypoints[1,0], waypoints[1,1], 0, 0, 0, 0])

    if Tang == 1:
        alphax = np.linalg.lstsq(Ax, bx, rcond=None)
    else:
        alphax = np.linalg.lstsq(Ay, bx, rcond=None)
    alphay = np.linalg.lstsq(Ay, by, rcond=None)
    Alphas = np.vstack((np.transpose(alphax[0]), np.transpose(alphay[0])))

else:  # Three or more waypoints
    A = np.zeros((n*6, n*6))
    for i in range(n):
        T = Ts[i]
        A1 = np.array([[1, 0, 0, 0, 0, 0],
                       [1, 1*T, 1*T**2, 1*T**3, 1*T**4, 1*T**5]])  # Conditions at waypoints
        A[2*i:2*i+2, 6*i:6*(i+1)] = A1

    T = Ts[-1]
    A2yi = np.array([[0, 1, 0, 0, 0, 0],  # Velocity at start
                     [0, 0, 2, 0, 0, 0]])  # Acceleration at start
    A2xi = np.array([0, 1, 0, 0, 0, 0])  # Velocity at start

    A2yf = np.array([[0, 1, 2*T, 3*T**2, 4*T**3, 5*T**4],  # Velocity at end
                     [0, 0, 2, 6*T, 12*T**2, 20*T**3]])  # Acceleration at end

    A2xf = np.array([[0, 1, 2*T, 3*T**2, 4*T**3, 5*T**4],  # Velocity at end
                     [0, 0, 2, 6*T, 12*T**2, 20*T**3],  # Acceleration at end
                     [0, 0, 0, 6, 24*T, 60*T**2]])  # Jerk at end

    Ax = np.copy(A)
    Ay = np.copy(A)
    Ay[2*n:2*n+2, 0:6] = A2yi
    Ay[2*n+2:2*n+4, 6*(n-1):6*n+1] = A2yf
    Ax[2*n, 0:6] = A2xi
    Ax[2*n+1:2*n+4, 6*(n-1):6*n+1] = A2xf

    for j in range(n-1):  # Connecting polynomial pairs
        T = Ts[j]
        A3 = np.array([[0, 1, 2*T, 3*T**2, 4*T**3, 5*T**4, 0, -1, 0, 0, 0, 0],
                       [0, 0, 2, 6*T, 12*T**2, 20*T**3, 0, 0, -2, 0, 0, 0],
                       [0, 0, 0, 6, 24*T, 60*T**2, 0, 0, 0, -6, 0, 0],
                       [0, 0, 0, 0, 24, 120*T, 0, 0, 0, 0, -24, 0]])
        Ax[2*n+4*(j)+4:2*n+4*(j)+8, 6*(j):6*(j)+12] = A3  # Continuity conditions
        Ay[2*n+4*(j)+4:2*n+4*(j)+8, 6*(j):6*(j)+12] = A3  # Continuity conditions

    bx = np.zeros((6*n, 1))
    by = np.copy(bx)
    for l in range(n):
        bx[2*l] = waypoints[0, l]
        bx[2*l+1] = waypoints[0, l+1]
        by[2*l] = waypoints[1, l]
        by[2*l+1] = waypoints[1, l+1]

    if Tang == 1:
        alphax = np.linalg.lstsq(Ax, bx, rcond=None)
    else:
        alphax = np.linalg.lstsq(Ay, bx, rcond=None)
    alphay = np.linalg.lstsq(Ay, by, rcond=None)
    Alphas = np.vstack((np.transpose(alphax[0]), np.transpose(alphay[0])))

# Generate Trajectory for VecT
c = np.zeros(np.size(VecT))
VecPos = np.zeros((2, np.size(VecT)))
VecVel = np.zeros((2, np.size(VecT)))
VecAccel = np.zeros((2, np.size(VecT)))
VecJerk = np.zeros((2, np.size(VecT)))
Psi = np.zeros((np.size(VecT)))
t = np.zeros(np.size(VecT))
for i in range(np.size(VecT)):
    c[i] = np.where(Si >= VecT[i])[0][0]
    if VecT[i] == 0:
        c[i] = 1
    k = int(c[i])
    t[i] = (VecT[i] - Si[k-1])
    VecPos[:,i] = Alphas[:,6*(k-1)] + Alphas[:,6*(k-1)+1]*t[i] + Alphas[:,6*(k-1)+2]*t[i]**2  + Alphas[:,6*(k-1)+3]*t[i]**3 + Alphas[:,6*(k-1)+4]*t[i]**4 + Alphas[:,6*(k-1)+5]*t[i]**5
    VecVel[:,i] = Alphas[:,6*(k-1) + 1] + 2*Alphas[:,6*(k-1) + 2]*t[i] + 3*Alphas[:,6*(k-1) + 3]*t[i]**2 + 4*Alphas[:,6*(k-1) + 4]*t[i]**3 + 5*Alphas[:,6*(k-1) + 5]*t[i]**4
    VecAccel[:,i] = 2*Alphas[:,6*(k-1) + 2] + 6*Alphas[:,6*(k-1) + 3]*t[i] + 12*Alphas[:,6*(k-1) + 4]*t[i]**2 + 20*Alphas[:,6*(k-1) + 5]*t[i]**3
    VecJerk[:,i] =  6*Alphas[:,6*(k-1) + 3] + 24*Alphas[:,6*(k-1) + 4]*t[i] + 60*Alphas[:,6*(k-1) + 5]*t[i]**2
    if i == 0:
        Psi[i] = 0
    else:
        Psi[i] = math.atan2(VecVel[1,i], VecVel[0,i])
if Tang == 1:
    Psi[0] = 0
else:
    Psi[0] = Psi[1]
Psi[-1] = Psi[-2]
Psi = np.unwrap(Psi)
Psi_dot = np.diff(Psi)/dT
Psi_dot[-1] = Psi_dot[-2]
Psi_dot = np.append(Psi_dot, Psi_dot[-1])
Psi_ddot = np.diff(Psi_dot)/dT
Psi_ddot[-2] = Psi_ddot[-3]
Psi_ddot[-1] = Psi_ddot[-2]
Psi_ddot = np.append(Psi_ddot, Psi_ddot[-1])

# Wheel position calculations
PosRodaR = np.zeros((2, np.size(VecT)))
PosRodaL = np.copy(PosRodaR)
VecRoda = np.copy(PosRodaL)
S = np.zeros(np.size(VecT))
Slast = 0
for i in range(np.size(VecT)):
    VecRoda[:,i:i+1] = Tr/2 * np.array([[math.sin(Psi[i])], [-math.cos(Psi[i])]])
    if i != 0:
        S[i] = Slast + np.linalg.norm((VecPos[:,i] - VecPos[:,i-1]))
        Slast = S[i]
    PosRodaR[:,i] = VecPos[:,i] + VecRoda[:,i]
    PosRodaL[:,i] = VecPos[:,i] - VecRoda[:,i]

# Wheel angular position calculations
Theta_R = (S + Psi*Tr/2)/R - Psi[0]*Tr/2/R
Theta_L = (S - Psi*Tr/2)/R + Psi[0]*Tr/2/R

# Wheel angular velocity and acceleration
Theta_R_dot = np.diff(Theta_R)/dT
Theta_R_dot[-1] = Theta_R_dot[-2]
Theta_R_dot = np.append(Theta_R_dot, Theta_R_dot[-1])
Theta_R_ddot = np.diff(Theta_R_dot)/dT
Theta_R_ddot[-2] = Theta_R_ddot[-3]
Theta_R_ddot[-1] = Theta_R_ddot[-2]
Theta_R_ddot = np.append(Theta_R_ddot, Theta_R_ddot[-1])
Theta_L_dot = np.diff(Theta_L)/dT
Theta_L_dot[-1] = Theta_L_dot[-2]
Theta_L_dot = np.append(Theta_L_dot, Theta_L_dot[-1])
Theta_L_ddot = np.diff(Theta_L_dot)/dT
Theta_L_ddot[-2] = Theta_L_ddot[-3]
Theta_L_ddot[-1] = Theta_L_ddot[-2]
Theta_L_ddot = np.append(Theta_L_ddot, Theta_L_ddot[-1])

# PD controller calculation
Kd = 0
Kp = 0
uR = 0 * VecT
lastuR = 0
y = Theta_R * (1 + 0.01) * np.random.randn(np.size(VecT))
for i in range(np.size(VecT)):
    uR[i] = 1/(Kd*dT + 1) * lastuR + dT/(Kd*dT + 1) * (Theta_R_ddot[i] + Kd*Theta_R_dot[i] + Kp*(Theta_R[i] - y[i]))
    lastuR = uR[i]

# Data preparation for saving
StrVecT = ','.join(map(str, VecT))
StrThetaL = ','.join(map(str, Theta_L))
StrThetaLd = ','.join(map(str, Theta_L_dot))
StrThetaLdd = ','.join(map(str, Theta_L_ddot))
StrThetaR = ','.join(map(str, Theta_R))
StrThetaRd = ','.join(map(str, Theta_R_dot))
StrThetaRdd = ','.join(map(str, Theta_R_ddot))

DadosL = f'{{{{{StrVecT}}},\n{{{StrThetaL}}},\n{{{StrThetaLd}}},\n{{{StrThetaLdd}}}}}'
DadosR = f'{{{{{StrVecT}}},\n{{{StrThetaR}}},\n{{{StrThetaRd}}},\n{{{StrThetaRdd}}}}}'
arrayBin = np.zeros((6, np.size(Theta_L)))
arrayBin[0, :] = Theta_L
arrayBin[1, :] = Theta_L_dot
arrayBin[2, :] = Theta_L_ddot
arrayBin[3, :] = Theta_R
arrayBin[4, :] = Theta_R_dot
arrayBin[5, :] = Theta_R_ddot
arrayBin = np.transpose(arrayBin)
arrayBin.astype('float32').tofile("output.bin")

with open('NavegaDadosL.txt', 'w') as fid:
    fid.write(DadosL)

with open('NavegaDadosR.txt', 'w') as fid:
    fid.write(DadosR)
