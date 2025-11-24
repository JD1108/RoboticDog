#%%
#imports, global functions and constants
import numpy as np
import matplotlib.pyplot as plt

def getPart(knot1, knot2):
    x=[knot1[0],knot2[0]]
    y=[knot1[1],knot2[1]]
    return x,y
def forwardKin(aH,aK):
    knee=(thighLength*np.cos(aH),thighLength*np.sin(aH))
    ankle=(knee[0]+lowerLegLength*np.cos(-aK+(np.pi+aH)),knee[1]+lowerLegLength*np.sin(-aK+(np.pi+aH)))
    return knee,ankle
def inverseKin(pos):
    d=np.sqrt(pos[0]**2+pos[1]**2)
    if d>thighLength+lowerLegLength:
        print("knee")
        return -np.pi/2

    aH=+np.arctan2(pos[1],pos[0])-np.arccos((thighLength**2+d**2-lowerLegLength**2)/(2*d*thighLength))
    aK=np.arccos((thighLength**2+lowerLegLength**2-d**2)/(2*lowerLegLength*thighLength))
    if aH>17/18*np.pi or aH<-17/18*np.pi:
        print("hip")
    return aH,aK
def moments(pos,height,f):
    if pos[1]>height:
        return 0,0
    else:
        knee,ankle=forwardKin(*inverseKin(pos))
        mH=(np.abs(pos[0])/1000)*f
        mK=(np.abs(knee[0]-ankle[0])/1000)*f
        return mH,mK
def trajectory(time, phaseOffset, stepTime, stepLength,height,stepHeight):
    phase = ((time / stepTime) + phaseOffset) % 1

    if phase < 0.75:
        x = (0.25 * stepLength) - (phase / 0.75) * stepLength
        y = height
    else:
        def bezier4(p0, p1, p2, p3, p4, t):
            return (
                    (1 - t) ** 4 * p0 +
                    4 * (1 - t) ** 3 * t * p1 +
                    6 * (1 - t) ** 2 * t ** 2 * p2 +
                    4 * (1 - t) * t ** 3 * p3 +
                    t ** 4 * p4
            )

        def swingX(p, start, end, speed):
            return (start + speed - end) / 2 * np.cos(np.pi * p) + speed * p + (start - (start + speed - end) / 2)

        p = (phase - 0.75) / 0.25
        p0 = np.array([-0.75*stepLength,height])
        p1 = np.array([-0.83*stepLength,height+0.1*stepHeight])
        p2 = np.array([0*stepLength,height+3*stepHeight])
        p3 = np.array([0.33*stepLength,height+0.1*stepHeight])
        p4 = np.array([0.25*stepLength,height])
        x,y = bezier4(p0, p1, p2, p3,p4, p)
        #x =  swingX(p,-0.75*stepLength,+0.25*stepLength,stepLength/stepTime)
    return [x, y]



thighLength=77
lowerLegLength=64 #64
hipPosition=(0,0)



#%%
# leg
footPosition=(-30,-135)
h, k=inverseKin(footPosition)
#h=-155/180*np.pi
#k=90/180*np.pi
knee,ankle=forwardKin(h,k)
print(knee,ankle)
fig, ax = plt.subplots()
fig.suptitle("2-DOF Leg")
ax.plot(*getPart(hipPosition,knee),"-o",color="#00028f")
ax.plot(*getPart(knee,ankle),"-o",color="#444")
ax.plot(*getPart(hipPosition,ankle),"--",color="grey")
ax.plot([0,40],[0,0],":",color="grey")
ax.plot([knee[0],knee[0]+40],[knee[1],knee[1]],":",color="grey")
ax.text(20,-10,f"{h*(180/np.pi):.0f}°")
ax.text(knee[0]+15,knee[1]-15,f"{k*(180/np.pi):.0f}°")
ax.text((ankle[0]/2)+0.5,ankle[1]/2,"d")
ax.spines['right'].set_visible(False)
ax.spines['bottom'].set_visible(False)
ax.set_xlim(-50, 50)
ax.set_ylim(-140, 5)
ax.set_aspect("equal")
ax.xaxis.tick_top()
plt.savefig("images/leg_ik.png",bbox_inches="tight",transparent=True)
plt.show()

#%%
#static moment for servos and simulation

height=-135
stepTime=1000
stepLength=40
stepHeight=10
deltaT=10
m=0.5
faktor=0.4
f=m*9.81*faktor
stallServo=0.177
limitServo=stallServo/3
results=np.empty((stepTime//deltaT,11)) #time, x, y, mH, mK, aH, aK, vAH,vAK, vX, vY
                                    #    0    1  2  3   4   5    6   7   8,    9,  10
for t, i in zip(range(0,stepTime,deltaT),range(0,stepTime//deltaT)):
    results[i,0]=t
    results[i,1],results[i,2]=trajectory(t,0,stepTime,stepLength,height,stepHeight)
    results[i,3],results[i,4]=moments([results[i,1],results[i,2]],height,f)
    aH,aK=inverseKin([results[i,1],results[i,2]])
    results[i,5]=((180/np.pi)*aH)
    results[i, 6]=((180/np.pi)*aK)
    if i>0:
        results[i,7]=(results[i,5]-results[i-1,5])/(results[i,0]-results[i-1,0])*1000
        results[i,8] = (results[i, 6] - results[i - 1, 6]) / (results[i, 0] - results[i - 1, 0])*1000
        results[i,9]= (results[i,1] - results[i - 1, 1]) / (results[i, 0] - results[i - 1, 0])*1000
        results[i,10]= (results[i,2] - results[i - 1, 2]) / (results[i, 0] - results[i - 1, 0])*1000

fig, axes = plt.subplots(nrows=5,ncols=2)
fig.suptitle("Simulation of a 2-DOF Leg")
axes[0,0].plot(results[:,0],results[:,1])
axes[0,0].set_ylabel("x [mm]")
axes[0,1].plot(results[:,0],results[:,2])
axes[0,1].set_ylabel("y [mm]")
axes[1,0].plot(results[:,0],results[:,9])
axes[1,0].set_ylabel("v x [mm/s]")
axes[1,1].plot(results[:,0],results[:,10])
axes[1,1].set_ylabel("v y [mm/s]")

axes[2,0].plot(results[:,0],results[:,3])
axes[2,0].plot([0,stepTime],[limitServo,limitServo],"-",color="red")
axes[2,0].set_ylabel("Hip M [Nm]")
axes[2,1].plot(results[:,0],results[:,4])
axes[2,1].plot([0,stepTime],[limitServo,limitServo],"-",color="red")
axes[2,1].set_ylabel("Knee M [Nm]")

axes[3,0].plot(results[:,0],results[:,5])
axes[3,0].set_ylabel("Hip A[°]")
axes[3,1].plot(results[:,0],results[:,6])
axes[3,1].set_ylabel("Knee A[°]")

axes[4,0].plot(results[:,0],results[:,7])
axes[4,0].set_ylabel("Hip W [°/s]")
axes[4,1].plot(results[:,0],results[:,8])
axes[4,1].set_ylabel("Knee W [°/s]")

for ax in axes.flat:
    ax.grid(True)
    ax.set_xlabel("time [ms]")
fig.tight_layout()

plt.savefig("images/leg_m.png",bbox_inches="tight",transparent=True)
plt.show()



#%%
height=-135
stepTime=1000
stepLength=40
stepHeight=10
deltaT=1

arr=np.empty((stepTime//deltaT,2))
for t in range(0,stepTime//deltaT):
    arr[t,0], arr[t,1] = trajectory(t*deltaT,0,stepTime,stepLength,height,stepHeight)

fig, ax = plt.subplots()
fig.suptitle("Trajectory of a 2-DOF Leg")
ax.plot(arr[:,0],arr[:,1])
ax.set_ylabel("x [mm]")
ax.set_xlabel("y [ms]")
plt.savefig("images/leg_tr.png",bbox_inches="tight",transparent=True)

plt.show()


#%%
# jump in velocity
arr=np.empty((200,3))
i=0
for t in np.linspace(stepTime//4*3-20,stepTime//4*3+20,200):
    arr[i, 0] = t
    arr[i, 1], arr[i, 2] = trajectory(t, 0, stepTime, stepLength, height, stepHeight)
    if i>0:
        arr[i, 2] = (arr[i,1]-arr[i-1,1])/(arr[i,0]-arr[i-1,0])*1000
    i+=1

fig, ax = plt.subplots()
ax.plot(arr[:,0],arr[:,2])
plt.show()

