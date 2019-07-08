#ifndef __PHYSENVIRONMENT_H__
#define __PHYSENVIRONMENT_H__

#include "pragma/networkdefinitions.h"
#include <mathutil/glmutil.h>
#include "pragma/physics/transform.hpp"
#include <sharedutils/functioncallback.h>
#include <sharedutils/util_shared_handle.hpp>
#include <pragma/math/vector/wvvector3.h>
#include <vector>
#include <unordered_map>
#ifdef __linux__
#include <BulletSoftBody/btSoftBody.h>
#include <BulletSoftBody/btSoftBodySolvers.h>
#endif

class NetworkState;
class BaseEntity;
class PhysObj;
struct TraceResult;
class TraceData;
struct PhysSoftBodyInfo;
enum class RayCastFlags : uint32_t;

namespace pragma::physics
{
	class IBase;
	class IShape;
	class IConvexShape;
	class IMaterial;
	class IConstraint;
	class IFixedConstraint;
	class IBallSocketConstraint;
	class IHingeConstraint;
	class ISliderConstraint;
	class IConeTwistConstraint;
	class IDoFConstraint;
	class IDoFSpringConstraint;

	class IController;
	class ICollisionObject;
	class IRigidBody;
	class ISoftBody;
	class IVehicle;
	
	class IGhostObject;
	class ICompoundShape;
	class IConvexHullShape;
	class ITriangleShape;

	class WaterBuoyancySimulator;
	class IVisualDebugger;

	using Scalar = double;
	class DLLNETWORK IEnvironment
	{
	public:
		enum class Event : uint32_t
		{
			OnConstraintCreated = 0,
			OnCollisionObjectCreated,
			OnControllerCreated,
			OnVehicleCreated,

			OnConstraintRemoved,
			OnCollisionObjectRemoved,
			OnControllerRemoved,
			OnVehicleRemoved,

			Count
		};
		static constexpr float DEFAULT_CHARACTER_SLOPE_LIMIT = 45.f;
		static constexpr std::string_view PHYS_ENGINE_MODULE_LOCATION = "physics_engines";
		static std::vector<std::string> GetAvailablePhysicsEngines();
		static std::string GetPhysicsEngineModuleLocation(const std::string &physEngine);
		template<class T,typename... TARGS>
			std::shared_ptr<T> CreateSharedPtr(TARGS&& ...args);
		template<class T,typename... TARGS>
			util::TSharedHandle<T> CreateSharedHandle(TARGS&& ...args);
		using RemainingDeltaTime = float;

		IEnvironment(NetworkState &state);
		virtual ~IEnvironment();
		NetworkState &GetNetworkState() const;
		double GetTimeScale() const;
		void AddEventCallback(Event eventid,const CallbackHandle &hCb);
		IMaterial &GetGenericMaterial() const;

		virtual bool Initialize();
		virtual float GetWorldScale() const;

		void SetVisualDebugger(std::unique_ptr<pragma::physics::IVisualDebugger> debugger);
		IVisualDebugger *GetVisualDebugger() const;

		virtual util::TSharedHandle<IFixedConstraint> CreateFixedConstraint(IRigidBody &a,const Vector3 &pivotA,const Quat &rotA,IRigidBody &b,const Vector3 &pivotB,const Quat &rotB)=0;
		virtual util::TSharedHandle<IBallSocketConstraint> CreateBallSocketConstraint(IRigidBody &a,const Vector3 &pivotA,IRigidBody &b,const Vector3 &pivotB)=0;
		virtual util::TSharedHandle<IHingeConstraint> CreateHingeConstraint(IRigidBody &a,const Vector3 &pivotA,IRigidBody &b,const Vector3 &pivotB,const Vector3 &axis)=0;
		virtual util::TSharedHandle<ISliderConstraint> CreateSliderConstraint(IRigidBody &a,const Vector3 &pivotA,const Quat &rotA,IRigidBody &b,const Vector3 &pivotB,const Quat &rotB)=0;
		virtual util::TSharedHandle<IConeTwistConstraint> CreateConeTwistConstraint(IRigidBody &a,const Vector3 &pivotA,const Quat &rotA,IRigidBody &b,const Vector3 &pivotB,const Quat &rotB)=0;
		virtual util::TSharedHandle<IDoFConstraint> CreateDoFConstraint(IRigidBody &a,const Vector3 &pivotA,const Quat &rotA,IRigidBody &b,const Vector3 &pivotB,const Quat &rotB)=0;
		virtual util::TSharedHandle<IDoFSpringConstraint> CreateDoFSpringConstraint(IRigidBody &a,const Vector3 &pivotA,const Quat &rotA,IRigidBody &b,const Vector3 &pivotB,const Quat &rotB)=0;

		virtual util::TSharedHandle<IController> CreateCapsuleController(float halfWidth,float halfHeight,float stepHeight,umath::Degree slopeLimit=DEFAULT_CHARACTER_SLOPE_LIMIT,const Transform &startTransform={})=0;
		virtual util::TSharedHandle<IController> CreateBoxController(const Vector3 &halfExtents,float stepHeight,umath::Degree slopeLimit=DEFAULT_CHARACTER_SLOPE_LIMIT,const Transform &startTransform={})=0;
		virtual util::TSharedHandle<ICollisionObject> CreateCollisionObject(IShape &shape)=0;
		virtual util::TSharedHandle<IRigidBody> CreateRigidBody(float mass,IShape &shape,const Vector3 &localInertia,bool dynamic=true)=0;
		virtual util::TSharedHandle<ISoftBody> CreateSoftBody(const PhysSoftBodyInfo &info,float mass,const std::vector<Vector3> &verts,const std::vector<uint16_t> &indices,std::vector<uint16_t> &indexTranslations)=0;
		virtual util::TSharedHandle<IGhostObject> CreateGhostObject(IShape &shape)=0;

		virtual std::shared_ptr<IConvexShape> CreateCapsuleShape(float halfWidth,float halfHeight,const IMaterial &mat)=0;
		virtual std::shared_ptr<IConvexShape> CreateBoxShape(const Vector3 &halfExtents,const IMaterial &mat)=0;
		virtual std::shared_ptr<IConvexShape> CreateCylinderShape(float radius,float height,const IMaterial &mat)=0;
		virtual std::shared_ptr<IConvexShape> CreateSphereShape(float radius,const IMaterial &mat)=0;
		virtual std::shared_ptr<IConvexHullShape> CreateConvexHullShape(const IMaterial &mat)=0;
		virtual std::shared_ptr<ITriangleShape> CreateTriangleShape(const IMaterial &mat)=0;
		std::shared_ptr<ICompoundShape> CreateCompoundShape();
		std::shared_ptr<ICompoundShape> CreateCompoundShape(IShape &shape);
		virtual std::shared_ptr<ICompoundShape> CreateCompoundShape(std::vector<IShape*> &shapes)=0;
		virtual std::shared_ptr<IShape> CreateHeightfieldTerrainShape(uint32_t width,uint32_t length,Scalar maxHeight,uint32_t upAxis,const IMaterial &mat)=0;
		virtual std::shared_ptr<IMaterial> CreateMaterial(float staticFriction,float dynamicFriction,float restitution)=0;

		virtual RemainingDeltaTime StepSimulation(float timeStep,int maxSubSteps=1,float fixedTimeStep=(1.f /60.f))=0;

		virtual Bool Overlap(const TraceData &data,std::vector<TraceResult> *optOutResults=nullptr) const=0;
		virtual Bool RayCast(const TraceData &data,std::vector<TraceResult> *optOutResults=nullptr) const=0;
		virtual Bool Sweep(const TraceData &data,std::vector<TraceResult> *optOutResults=nullptr) const=0;

		const std::vector<util::TSharedHandle<IConstraint>> &GetConstraints() const;
		std::vector<util::TSharedHandle<IConstraint>> &GetConstraints();
		const std::vector<util::TSharedHandle<ICollisionObject>> &GetCollisionObjects() const;
		std::vector<util::TSharedHandle<ICollisionObject>> &GetCollisionObjects();
		const std::vector<util::TSharedHandle<IController>> &GetControllers() const;
		std::vector<util::TSharedHandle<IController>> &GetControllers();

		virtual void RemoveConstraint(IConstraint &constraint);
		virtual void RemoveCollisionObject(ICollisionObject &obj);
		virtual void RemoveController(IController &controller);
		virtual void RemoveVehicle(IVehicle &vehicle);

		const WaterBuoyancySimulator &GetWaterBuoyancySimulator() const;
	protected:
		friend IConstraint;
		friend ICollisionObject;
		friend IController;
		void ClearConstraints();
		void ClearCollisionObjects();
		void ClearControllers();
		void ClearVehicles();

		std::vector<util::TSharedHandle<IConstraint>> m_constraints = {};
		std::vector<util::TSharedHandle<ICollisionObject>> m_collisionObjects = {};
		std::vector<util::TSharedHandle<IController>> m_controllers = {};
		std::vector<util::TSharedHandle<IVehicle>> m_vehicles = {};

		void AddConstraint(IConstraint &constraint);
		void AddCollisionObject(ICollisionObject &obj);
		void AddController(IController &controller);
		void AddVehicle(IVehicle &vehicle);

		template<class T>
			void CallCallbacks(Event eventid,T &obj);
	protected:
		std::unique_ptr<pragma::physics::IVisualDebugger> m_visualDebugger = nullptr;
	private:
		NetworkState &m_nwState;
		std::unordered_map<Event,std::vector<CallbackHandle>> m_callbacks = {};
		std::shared_ptr<WaterBuoyancySimulator> m_buoyancySim = nullptr;
		std::shared_ptr<IMaterial> m_genericMaterial = nullptr;
	};
};

template<class T,typename... TARGS>
std::shared_ptr<T> pragma::physics::IEnvironment::CreateSharedPtr(TARGS&& ...args)
{
	auto ptr = std::shared_ptr<T>{
		new T{std::forward<TARGS>(args)...},[](T *o) {
			o->OnRemove();
			delete o;
		}
	};
	ptr->Initialize();
	ptr->InitializeLuaObject(GetNetworkState().GetLuaState());
	return ptr;
}
template<class T,typename... TARGS>
util::TSharedHandle<T> pragma::physics::IEnvironment::CreateSharedHandle(TARGS&& ...args)
{
	auto handle = util::TSharedHandle<T>{
		new T{std::forward<TARGS>(args)...},[](T *o) {
			o->OnRemove();
			delete o;
		}
	};
	handle->Initialize();
	handle->InitializeLuaHandle(GetNetworkState().GetLuaState(),util::TWeakSharedHandle<IBase>{util::shared_handle_cast<T,IBase>(handle)});
	return handle;
}

template<class T>
	void pragma::physics::IEnvironment::CallCallbacks(Event eventid,T &obj)
{
	auto itCb = m_callbacks.find(eventid);
	if(itCb == m_callbacks.end())
		return;
	auto &v = itCb->second;
	for(auto it=v.begin();it!=v.end();)
	{
		auto &hCallback = *it;
		if(hCallback.IsValid() == false)
		{
			it = v.erase(it);
			continue;
		}
		hCallback.Call<void,T*>(&obj);
		++it;
	}
}

#endif