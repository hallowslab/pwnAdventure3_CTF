#define uint32_t unsigned int
#define size_t unsigned long
#define uint8_t unsigned char
#define int32_t int


class IAchievement;
class IQuestState;
class IQuest;
class NPC;
class AIZone;
class AIZoneListener;
class Spawner;
class IFastTravel;
class Projectile;
class IUE4Actor;
class ILocalPlayer;
class IItem;
class IPlayer;
class IActor;
class Actor;
class Player;
class GameAPI;
class World;
class TimerSet;
struct LocationAndRotation;
struct QuestStateInfo;
struct PlayerQuestState;
struct ItemAndCount;

enum DamageType {PhysicalDamage, FireDamage, ColdDamage, ShockDamage};
enum ItemRarity {ResourceItem, NormalItem, RareItem, LegendaryItem, LeetItem};
enum NPCStateTransitionType {EndConversationTransition, ContinueConversationTransition, ShopTransition};

template <class T> struct ActorRef {
    T *m_object;

    ActorRef();
    ActorRef(T *);
    ActorRef(const ActorRef<T> &);
    ActorRef<T> & operator=(T *);
    ActorRef<T> & operator=(const ActorRef<T> &);
    T * operator->() const;
    T * Get() const;
    bool operator<(const ActorRef<T> &) const;
};

struct PlayerQuestState {
    IQuestState *state;
    uint32_t count;
};

struct NPCStateTransition {
    std::string text;
    NPCStateTransitionType type;
    std::string nextState;
};

struct NPCState {
    std::string text;
    std::vector<NPCStateTransition> transitions;
};

struct ItemCountInfo {
    uint32_t count;
    uint32_t loadedAmmo;
};

struct Vector3 {
    float x;
    float y;
    float z;
  public:
    Vector3();
    Vector3(float, float, float);
    Vector3 operator*(float) const;
    Vector3 & operator*=(float);
    Vector3 operator+(const Vector3 &) const;
    Vector3 & operator+=(const Vector3 &);
    Vector3 operator-(const Vector3 &) const;
    Vector3 & operator-=(const Vector3 &);
    float MagnitudeSquared() const;
    float Magnitude() const;
    static float DistanceSquared(const Vector3 &, const Vector3 &);
    static float Distance(const Vector3 &, const Vector3 &);
    void Normalize();
    static Vector3 Normalize(const Vector3 &);
};

struct Rotation {
    float pitch;
    float yaw;
    float roll;
  public:
    Rotation();
    Rotation(float, float, float);
};

class IUE4Actor {
  public:
    virtual void * GetUE4Actor();
    virtual void RemoveFromWorld();
    virtual Vector3 GetPosition();
    virtual Rotation GetRotation();
    virtual Vector3 GetProjectilePosition();
    virtual Vector3 GetCharacterVelocity();
    virtual void SetPosition(const Vector3 &);
    virtual void SetRotation(const Rotation &);
    virtual void SetCharacterVelocity(const Vector3 &);
    virtual void SetForwardAndStrafeMovement(float, float);
    virtual void InterpolatePositionAndRotation(const Vector3 &, const Rotation &, float, float);
    virtual bool MoveToLocation(const Vector3 &);
    virtual bool MoveToRandomLocationInRadius(float);
    virtual bool MoveToActor(IActor *);
    virtual void OnUpdateState(const char *, bool);
    virtual void OnTriggerEvent(const char *, IActor *);
    virtual void OnUpdatePvPEnabled(bool);
    virtual IActor * LineTraceTo(const Vector3 &);
    virtual void FireBullets(IItem *, int32_t, DamageType, const Vector3 &, uint32_t, float);
    virtual void LocalRespawn(const Vector3 &, const Rotation &);
    virtual bool IsOnGround();
    virtual void OnReload(uint32_t);
};

class ILocalPlayer : public IUE4Actor {
  public:
    virtual void SetPlayerInterface(IPlayer *);
    virtual Vector3 GetLookPosition();
    virtual Rotation GetLookRotation();
    virtual float GetForwardMovementFraction();
    virtual float GetStrafeMovementFraction();
    virtual void SetCurrentQuest(IQuest *, IQuestState *, uint32_t);
    virtual void DisplayMessage(const char *, const char *);
    virtual void DisplayEvent(const char *, const char *);
    virtual void OnEquip(size_t, IItem *);
    virtual void OnChangeSlot(size_t);
    virtual void OnUpdateCountdown(int32_t);
    virtual void OnUpdatePvPCountdown(bool, int32_t);
    virtual void OnNewItem(const char *, uint32_t);
    virtual void OnNPCConversationState(IActor *, const char *, const char **, const char **, size_t);
    virtual void OnNPCConversationEnd();
    virtual void OnNPCShop(IActor *);
    virtual void OnChatMessage(const char *, bool, const char *);
    virtual void OnPlayerKillMessage(const char *, bool, const char *, bool, IItem *);
    virtual void OnPlayerSuicideMessage(const char *, bool, IItem *);
    virtual void OnPlayerDeadMessage(const char *, bool, const char *);
    virtual void OnAchievement(const char *);
    virtual void OnLocalDeath(IActor *, IItem *);
};

class IInventory {
  public:
    virtual ~IInventory();
    virtual size_t GetCount();
    virtual IItem * GetItem(size_t);
    virtual uint32_t GetItemCount(size_t);
    virtual uint32_t GetItemLoadedAmmo(size_t);
    virtual void Destroy();
};

class IPlayer {
  public:
    virtual IActor * GetActorInterface();
    void AddRef();
    void Release();
    virtual bool IsLocalPlayer() const;
    virtual ILocalPlayer * GetLocalPlayer() const;
    virtual const char * GetPlayerName();
    virtual const char * GetTeamName();
    virtual uint8_t GetAvatarIndex();
    virtual const uint32_t * GetColors();
    virtual bool IsPvPDesired();
    virtual void SetPvPDesired(bool);
    virtual IInventory * GetInventory();
    virtual uint32_t GetItemCount(IItem *);
    virtual uint32_t GetLoadedAmmo(IItem *);
    virtual bool AddItem(IItem *, uint32_t, bool);
    virtual bool RemoveItem(IItem *, uint32_t);
    virtual bool AddLoadedAmmo(IItem *, IItem *, uint32_t);
    virtual bool RemoveLoadedAmmo(IItem *, uint32_t);
    virtual IItem * GetItemForSlot(size_t);
    virtual void EquipItem(size_t, IItem *);
    virtual size_t GetCurrentSlot();
    virtual void SetCurrentSlot(size_t);
    virtual IItem * GetCurrentItem();
    virtual int32_t GetMana();
    virtual bool UseMana(int32_t);
    virtual void SetItemCooldown(IItem *, float, bool);
    virtual bool IsItemOnCooldown(IItem *);
    virtual float GetItemCooldown(IItem *);
    virtual bool HasPickedUp(const char *);
    virtual void MarkAsPickedUp(const char *);
    virtual IQuest ** GetQuestList(size_t *);
    virtual void FreeQuestList(IQuest **);
    virtual IQuest * GetCurrentQuest();
    virtual void SetCurrentQuest(IQuest *);
    virtual PlayerQuestState GetStateForQuest(IQuest *);
    virtual void StartQuest(IQuest *);
    virtual void AdvanceQuestToState(IQuest *, IQuestState *);
    virtual void CompleteQuest(IQuest *);
    virtual bool IsQuestStarted(IQuest *);
    virtual bool IsQuestCompleted(IQuest *);
    virtual void EnterAIZone(const char *);
    virtual void ExitAIZone(const char *);
    virtual void UpdateCountdown(int32_t);
    void HideCountdown();
    virtual bool CanReload();
    virtual void RequestReload();
    virtual float GetWalkingSpeed();
    virtual float GetSprintMultiplier();
    virtual float GetJumpSpeed();
    virtual float GetJumpHoldTime();
    virtual bool CanJump();
    virtual void SetJumpState(bool);
    virtual void SetSprintState(bool);
    virtual void SetFireRequestState(bool);
    virtual void TransitionToNPCState(const char *);
    virtual void BuyItem(IActor *, IItem *, uint32_t);
    virtual void SellItem(IActor *, IItem *, uint32_t);
    virtual void EnterRegion(const char *);
    virtual void Respawn();
    virtual void Teleport(const char *);
    virtual void Chat(const char *);
    virtual IFastTravel * GetFastTravelDestinations(const char *);
    virtual void FastTravel(const char *, const char *);
    virtual void MarkAsAchieved(IAchievement *);
    virtual bool HasAchieved(IAchievement *);
    virtual void SubmitDLCKey(const char *);
    virtual uint32_t GetCircuitInputs(const char *);
    virtual void SetCircuitInputs(const char *, uint32_t);
    virtual void GetCircuitOutputs(const char *, bool *, size_t);
};

class IFastTravel {
  public:
    virtual ~IFastTravel();
    virtual size_t GetCount();
    virtual const char * GetRegionName(size_t);
    virtual const char * GetDisplayName(size_t);
    virtual void Destroy();
};

class IAchievement {
  public:
    virtual const char * GetName();
    virtual const char * GetDisplayName();
    virtual const char * GetDescription();
};

class IActor {
  public:
    virtual ~IActor();
    virtual void * GetUE4Actor();
    virtual bool IsNPC();
    virtual bool IsPlayer();
    virtual IPlayer * GetPlayerInterface();
    virtual void AddRef();
    virtual void Release();
    virtual void OnSpawnActor(IUE4Actor *);
    virtual void OnDestroyActor();
    virtual const char * GetBlueprintName();
    virtual bool IsCharacter();
    virtual bool CanBeDamaged(IActor *);
    virtual int32_t GetHealth();
    virtual int32_t GetMaxHealth();
    virtual void Damage(IActor *, IItem *, int32_t, DamageType);
    virtual void Tick(float);
    virtual bool CanUse(IPlayer *);
    virtual void OnUse(IPlayer *);
    virtual void OnHit(IActor *, const Vector3 &, const Vector3 &);
    virtual void OnAIMoveComplete();
    virtual const char * GetDisplayName();
    virtual bool IsElite();
    virtual bool IsPvPEnabled();
    virtual IItem ** GetShopItems(size_t &);
    virtual void FreeShopItems(IItem **);
    virtual int32_t GetBuyPriceForItem(IItem *);
    virtual int32_t GetSellPriceForItem(IItem *);
    virtual Vector3 GetLookPosition();
    virtual Rotation GetLookRotation();
    virtual IActor * GetOwner();
};

class IQuestState {
  public:
    virtual const char * GetName();
    virtual const char * GetDescription();
    virtual void CheckForEarlyCompletion(IPlayer *);
    virtual void OnItemAcquired(IPlayer *, IItem *);
    virtual void OnItemPickupUsed(IPlayer *, const char *);
};

class IQuest {
  public:
    virtual const char * GetName();
    virtual const char * GetDescription();
    virtual IQuestState * GetStartingState();
    virtual IQuestState * GetStateByName(const char *);
};


class IItem {
  public:
    virtual ~IItem();
    virtual const char * GetName();
    virtual const char * GetDisplayName();
    virtual const char * GetItemTypeName();
    virtual const char * GetDescription();
    virtual const char * GetFlavorText();
    virtual bool CanEquip();
    virtual uint32_t GetMaximumCount();
    virtual bool CanActivate(IPlayer *);
    virtual bool CanActivateInInventory();
    virtual void Activate(IPlayer *);
    virtual bool ShowInInventory();
    virtual bool ShowEventOnPickup();
    virtual bool ShowEventOnDuplicatePickup();
    virtual bool ShowNotificationOnPickup();
    virtual float GetCooldownTime();
    virtual ItemRarity GetItemRarity();
    virtual IItem * GetAmmoType();
    virtual uint32_t GetClipSize();
    virtual int32_t GetDamage();
    virtual int32_t GetDamagePerSecond();
    virtual DamageType GetDamageType();
    virtual int32_t GetManaCost();
    virtual const char * GetCustomCostDescription();
    virtual bool IsAutoFire();
    virtual uint32_t GetNumberOfProjectiles();
    virtual float GetReloadTime(int32_t);
    virtual bool HasPartialReload();
    virtual float GetRange();
    virtual int32_t GetTradeValue();
    virtual bool IsDynamic();
    virtual bool IsUpdating();
};

class AIZoneListener {
  private:
    AIZone *m_zone;

  public:
    AIZoneListener();
    virtual ~AIZoneListener();
    void SetAIZone(const std::string &);
    virtual void OnAIZoneActivated();
    virtual void OnAIZoneDeactivated();
    bool IsAIZoneActive();
};

class AIZone {
  private:
    std::string m_name;
    size_t m_playerCount;
    std::set<AIZoneListener*> m_listeners;

  public:
    AIZone(const std::string &);
    const std::string & GetName() const;
    bool IsActive() const;
    void OnPlayerEntered();
    void OnPlayerLeft();
    void AddListener(AIZoneListener *);
    void RemoveListener(AIZoneListener *);
};

class Spawner : public AIZoneListener {
  protected:
    std::vector<ActorRef<Actor>> m_actors;
    Vector3 m_position;
    Rotation m_rotation;
    size_t m_maxActors;
    float m_maxSpawnTimer;
    float m_currentSpawnTimer;

  public:
    Spawner(const std::string &, const Vector3 &, const Rotation &, size_t, float);
    virtual void OnAIZoneActivated();
    virtual void OnAIZoneDeactivated();
    virtual void Tick(float);
    virtual Actor * Spawn();
    void RemoveActor(Actor *);
    virtual size_t GetMaxActors();
    virtual float GetSpawnTimer();
};

class Actor : public IActor {
  protected:
    size_t m_refs;
    uint32_t m_id;
    IUE4Actor *m_target;
    TimerSet *m_timers;
    std::string m_blueprintName;
    ActorRef<IActor> m_owner;
    int32_t m_health;
    std::map<std::string, bool> m_states;
    float m_forwardMovementFraction;
    float m_strafeMovementFraction;
    Vector3 m_remotePosition;
    Vector3 m_remoteVelocity;
    Rotation m_remoteRotation;
    float m_remoteLocationBlendFactor;
    Spawner *m_spawner;

    virtual void OnKilled(IActor *, IItem *);
    virtual void OnTargetKilled(IActor *, IItem *);
  public:
    Actor(const std::string &);
    virtual ~Actor();
    virtual bool IsValid() const;
    virtual void * GetUE4Actor();
    virtual void AddRef();
    virtual void Release();
    void RemoveFromWorld();
    virtual void OnSpawnActor(IUE4Actor *);
    virtual void OnDestroyActor();
    virtual std::string GetDeathMessage();
    virtual const char * GetBlueprintName();
    virtual bool IsCharacter();
    virtual bool IsNPC();
    virtual bool IsProjectile();
    virtual bool IsPlayer();
    virtual IPlayer * GetPlayerInterface();
    virtual bool ShouldSendPositionUpdates();
    virtual bool ShouldReceivePositionUpdates();
    uint32_t GetId() const;
    void SetId(uint32_t);
    Vector3 GetPosition();
    Vector3 GetProjectilePosition();
    virtual Vector3 GetLookPosition();
    Rotation GetRotation();
    virtual Rotation GetLookRotation();
    Vector3 GetVelocity();
    float GetForwardMovementFraction() const;
    float GetStrafeMovementFraction() const;
    bool IsOnGround();
    void SetPosition(const Vector3 &);
    void SetRotation(const Rotation &);
    void SetVelocity(const Vector3 &);
    void SetForwardAndStrafeMovement(float, float);
    void SetRemotePositionAndRotation(const Vector3 &, const Rotation &);
    void InterpolateRemotePosition(float);
    virtual IActor * GetOwner();
    void LocalRespawn(const Vector3 &, const Rotation &);
    bool MoveToLocation(const Vector3 &);
    bool MoveToRandomLocationInRadius(float);
    bool MoveToActor(IActor *);
    bool GetState(const std::string &);
    virtual void UpdateState(const std::string &, bool);
    virtual void TriggerEvent(const std::string &, IActor *, bool);
    const std::map<std::string, bool> & GetStates();
    IActor * LineTraceTo(const Vector3 &);
    void FireBullets(IItem *, int32_t, DamageType, float, uint32_t, float);
    void FireBullets(IItem *, int32_t, DamageType, const Vector3 &, float, uint32_t, float);
    virtual bool CanBeDamaged(IActor *);
    virtual float GetMaximumDamageDistance();
    virtual int32_t GetHealth();
    virtual int32_t GetMaxHealth();
    virtual void Damage(IActor *, IItem *, int32_t, DamageType);
    void PerformSetHealth(int32_t);
    virtual void Tick(float);
    virtual bool CanUse(IPlayer *);
    virtual void OnUse(IPlayer *);
    virtual void PerformUse(IPlayer *);
    virtual void OnHit(IActor *, const Vector3 &, const Vector3 &);
    virtual void OnAIMoveComplete();
    virtual const char * GetDisplayName();
    virtual bool IsElite();
    virtual bool IsPvPEnabled();
    virtual IItem ** GetShopItems(size_t &);
    virtual std::vector<IItem*> GetShopItems();
    virtual void FreeShopItems(IItem **);
    virtual std::vector<IItem*> GetValidBuyItems();
    virtual float GetShopBuyPriceMultiplier();
    virtual float GetShopSellPriceMultiplier();
    virtual int32_t GetBuyPriceForItem(IItem *);
    virtual int32_t GetSellPriceForItem(IItem *);
    void SetSpawner(Spawner *);
    void AddTimer(const std::string &, float, const std::function<void ()> &);
    void AddTimerWithContext(const std::string &, float, const std::function<void (Actor *)> &);
    void AddRecurringTimer(const std::string &, float, const std::function<void ()> &);
    void AddRecurringTimerWithContext(const std::string &, float, const std::function<void (Actor *)> &);
    void CancelTimer(const std::string &);
    void PerformReloadNotification(uint32_t);
};

class TimerSet {
  public:
    struct TimerEvent {
        float timeLeft;
        float initialTime;
        bool recurring;
        bool withContext;
        std::function<void()> callback;
        std::function<void(Actor *)> contextCallback;
    };
    std::map<std::string, TimerSet::TimerEvent> m_timers;

  public:
    void Add(const std::string &, float, const std::function<void()> &);
    void AddWithContext(const std::string &, float, const std::function<void(Actor *)> &);
    void AddRecurring(const std::string &, float, const std::function<void()> &);
    void AddRecurringWithContext(const std::string &, float, const std::function<void(Actor *)> &);
    void Cancel(const std::string &);
    void Clear();
    void Tick(Actor *, float);
};

class NPC : public Actor {
  private:
    std::map<std::string, NPCState> m_states;

  public:
    NPC(const std::string &);
    virtual bool IsNPC();
    void AddState(const std::string &, const std::string &);
    void AddStateTransition(const std::string &, const std::string &, const std::string &);
    void AddStateTransitionToEnd(const std::string &, const std::string &);
    void AddStateTransitionToShop(const std::string &, const std::string &);
    std::string GetTextForState(const std::string &);
    std::vector<NPCStateTransition> GetTransitionsForState(const std::string &);
    virtual std::string GetInitialState(IPlayer *);
    virtual void OnTransitionTaken(IPlayer *, const std::string &, const std::string &);
    virtual bool CanUse(IPlayer *);
    virtual void PerformUse(IPlayer *);
    virtual int32_t GetBuyPriceForItem(IItem *);
    virtual int32_t GetSellPriceForItem(IItem *);
};

class Socket {
  protected:
    std::string m_lastErrorMessage;

  public:
    virtual ~Socket();
    virtual bool Read(void *, size_t);
    virtual bool Write(const void *, size_t);
    void ReadChecked(void *, size_t);
    uint8_t Read8();
    uint16_t Read16();
    uint32_t Read32();
    uint64_t Read64();
    std::string ReadString();
    float ReadFloat();
    Vector3 ReadVector();
    Vector3 ReadVector16();
    Rotation ReadRotation();
    Rotation ReadPrecisionRotation();
    float ReadSignedFraction();
    const std::string & GetLastErrorMessage() const;
};

class WriteStream {
  private:
    Socket *m_sock;
    std::vector<unsigned char> m_buffer;

  public:
    WriteStream(Socket *);
    void SetSocket(Socket *);
    void Write8(uint8_t);
    void Write16(uint16_t);
    void Write32(uint32_t);
    void Write64(uint64_t);
    void WriteSaturated16(float);
    void WriteString(const std::string &);
    void WriteFloat(float);
    void WriteVector(const Vector3 &);
    void WriteVector16(const Vector3 &);
    void WriteRotation(const Rotation &);
    void WritePrecisionRotation(const Rotation &);
    void WriteSignedFraction(float);
    void Write(const WriteStream &);
    void Write(const void *, size_t);
    void Flush();
    void Clear();
};

class Player : public Actor, public IPlayer {
  private:
    uint32_t m_characterId;
    std::string m_playerName;
    std::string m_teamName;
    uint8_t m_avatarIndex;
    uint32_t m_colors[4];
    std::map<IItem*, ItemAndCount> m_inventory;
    std::set<std::string> m_pickups;
    std::map<IItem*, float> m_cooldowns;
    std::map<std::string, unsigned int> m_circuitInputs;
    std::map<std::string, std::vector<std::allocator<bool>>> m_circuitOutputs;
    bool m_admin;
    bool m_pvpEnabled;
    bool m_pvpDesired;
    float m_pvpChangeTimer;
    int32_t m_pvpChangeReportedTimer;
    bool m_changingServerRegion;
    std::string m_currentRegion;
    std::string m_changeRegionDestination;
    std::set<std::string> m_aiZones;
    int32_t m_mana;
    float m_manaRegenTimer;
    float m_healthRegenCooldown;
    float m_healthRegenTimer;
    int32_t m_countdown;
    Vector3 m_remoteLookPosition;
    Rotation m_remoteLookRotation;
    IItem *m_equipped[10];
    size_t m_currentSlot;
    std::map<IQuest*, PlayerQuestState> m_questStates;
    IQuest *m_currentQuest;
    float m_walkingSpeed;
    float m_jumpSpeed;
    float m_jumpHoldTime;
    ActorRef<NPC> m_currentNPC;
    std::string m_currentNPCState;
    ILocalPlayer *m_localPlayer;
    WriteStream *m_eventsToSend;
    bool m_itemsUpdated;
    float m_itemSyncTimer;
    uint32_t m_chatMessageCounter;
    float m_chatFloodDecayTimer;
    IItem *m_lastHitByItem;
    float m_lastHitItemTimeLeft;
    float m_circuitStateCooldownTimer;

  protected:
    virtual void OnKilled(IActor *, IItem *);
  public:
    Player(bool);
    virtual ~Player();
    virtual bool IsPlayer();
    virtual IPlayer * GetPlayerInterface();
    virtual IActor * GetActorInterface();
    virtual bool CanBeDamaged(IActor *);
    virtual bool IsCharacter();
    virtual bool ShouldSendPositionUpdates();
    virtual bool ShouldReceivePositionUpdates();
    virtual void Tick(float);
    virtual void Damage(IActor *, IItem *, int32_t, DamageType);
    virtual void OnDestroyActor();
    void OnKillEvent(IPlayer *, IActor *, IItem *);
    virtual Vector3 GetLookPosition();
    virtual Rotation GetLookRotation();
    void SetRemoteLookPosition(const Vector3 &);
    void SetRemoteLookRotation(const Rotation &);
    virtual bool CanJump();
    virtual bool IsLocalPlayer() const;
    virtual ILocalPlayer * GetLocalPlayer() const;
    void InitLocalPlayer(ILocalPlayer *);
    bool IsAdmin() const;
    void SetPlayerName(const std::string &);
    void SetTeamName(const std::string &);
    void SetAvatarIndex(uint8_t);
    void SetColors(const uint32_t *);
    void SetCharacterId(uint32_t);
    virtual bool IsPvPEnabled();
    virtual bool IsPvPDesired();
    virtual void SetPvPDesired(bool);
    void PerformSetPvPEnabled(bool);
    void PerformSetPvPDesired(bool);
    void PerformUpdatePvPCountdown(bool, int32_t);
    virtual void UpdateState(const std::string &, bool);
    virtual const char * GetPlayerName();
    virtual const char * GetTeamName();
    virtual uint8_t GetAvatarIndex();
    virtual const uint32_t * GetColors();
    uint32_t GetCharacterId() const;
    const std::map<IItem*, ItemAndCount> & GetItemList() const;
    virtual IInventory * GetInventory();
    virtual uint32_t GetItemCount(IItem *);
    virtual uint32_t GetLoadedAmmo(IItem *);
    virtual bool AddItem(IItem *, uint32_t, bool);
    virtual bool RemoveItem(IItem *, uint32_t);
    bool PerformAddItem(IItem *, uint32_t, bool);
    bool PerformRemoveItem(IItem *, uint32_t);
    virtual bool AddLoadedAmmo(IItem *, IItem *, uint32_t);
    virtual bool RemoveLoadedAmmo(IItem *, uint32_t);
    void PerformSetLoadedAmmo(IItem *, uint32_t);
    virtual IItem * GetItemForSlot(size_t);
    virtual void EquipItem(size_t, IItem *);
    void PerformEquipItem(size_t, IItem *);
    virtual size_t GetCurrentSlot();
    virtual IItem * GetCurrentItem();
    virtual void SetCurrentSlot(size_t);
    void PerformSetCurrentSlot(size_t);
    void SetRemoteItem(IItem *);
    virtual int32_t GetMana();
    virtual bool UseMana(int32_t);
    void PerformSetMana(int32_t);
    virtual void SetItemCooldown(IItem *, float, bool);
    virtual bool IsItemOnCooldown(IItem *);
    virtual float GetItemCooldown(IItem *);
    virtual bool HasPickedUp(const char *);
    virtual void MarkAsPickedUp(const char *);
    void PerformMarkAsPickedUp(const std::string &);
    virtual IQuest ** GetQuestList(size_t *);
    virtual void FreeQuestList(IQuest **);
    virtual IQuest * GetCurrentQuest();
    virtual PlayerQuestState GetStateForQuest(IQuest *);
    virtual bool IsQuestStarted(IQuest *);
    virtual bool IsQuestCompleted(IQuest *);
    virtual void SetCurrentQuest(IQuest *);
    virtual void StartQuest(IQuest *);
    virtual void AdvanceQuestToState(IQuest *, IQuestState *);
    virtual void CompleteQuest(IQuest *);
    void PerformSetCurrentQuest(IQuest *);
    void PerformStartQuest(IQuest *);
    void PerformAdvanceQuestToState(IQuest *, IQuestState *);
    void PerformCompleteQuest(IQuest *);
    void SetInitialQuestStates(const std::map<std::string, QuestStateInfo> &, const std::string &);
    void SetInitialItemState(const std::map<std::string, ItemCountInfo> &, const std::vector<std::string> &, uint8_t);
    void SetInitialPickupState(const std::set<std::string> &);
    virtual void EnterAIZone(const char *);
    virtual void ExitAIZone(const char *);
    virtual void UpdateCountdown(int32_t);
    void PerformUpdateCountdown(int32_t);
    virtual void TriggerEvent(const std::string &, IActor *, bool);
    virtual bool CanReload();
    virtual void RequestReload();
    void PerformRequestReload();
    virtual float GetWalkingSpeed();
    virtual float GetSprintMultiplier();
    virtual float GetJumpSpeed();
    virtual float GetJumpHoldTime();
    virtual void SetJumpState(bool);
    virtual void SetSprintState(bool);
    virtual void SetFireRequestState(bool);
    void SetCurrentNPCState(NPC *, const std::string &);
    void EndNPCConversation();
    void EnterNPCShop(NPC *);
    NPC * GetCurrentNPC() const;
    const std::string & GetCurrentNPCState() const;
    virtual void TransitionToNPCState(const char *);
    void PerformTransitionToNPCState(const std::string &);
    virtual void BuyItem(IActor *, IItem *, uint32_t);
    void PerformBuyItem(IActor *, IItem *, uint32_t);
    virtual void SellItem(IActor *, IItem *, uint32_t);
    void PerformSellItem(IActor *, IItem *, uint32_t);
    virtual void EnterRegion(const char *);
    bool IsChangingRegion() const;
    const std::string & GetChangeRegionDestination() const;
    void PerformEnterRegion(const std::string &);
    LocationAndRotation GetSpawnLocation();
    virtual void Respawn();
    void PerformRespawn();
    void PerformRespawnAtLocation(const Vector3 &, const Rotation &);
    virtual void Teleport(const char *);
    void PerformTeleport(const std::string &);
    virtual void SendEvent(const WriteStream &);
    virtual void WriteAllEvents(WriteStream &);
    void SyncItems();
    virtual void Chat(const char *);
    void PerformChat(const std::string &);
    void ReceiveChat(Player *, const std::string &);
    virtual IFastTravel * GetFastTravelDestinations(const char *);
    virtual void FastTravel(const char *, const char *);
    void PerformFastTravel(const std::string &, const std::string &);
    void OnTravelComplete(const std::string &);
    IItem * GetLastHitByItem() const;
    void PerformSetLastHitByItem(IItem *);
    virtual void MarkAsAchieved(IAchievement *);
    virtual bool HasAchieved(IAchievement *);
    virtual void SubmitDLCKey(const char *);
    void PerformSubmitDLCKey(const std::string &);
    virtual uint32_t GetCircuitInputs(const char *);
    virtual void SetCircuitInputs(const char *, uint32_t);
    void PerformSetCircuitInputs(const std::string &, uint32_t);
    virtual void GetCircuitOutputs(const char *, bool *, size_t);
    void PerformSetCircuitOutputs(const std::string &, std::vector<std::allocator<bool>>);
    void InitCircuitStates();
};

class World {
  protected:
    std::set<ActorRef<IPlayer>> m_players;
    std::set<ActorRef<IActor>> m_actors;
    std::map<unsigned int, ActorRef<IActor>> m_actorsById;
    ILocalPlayer *m_localPlayer;
    uint32_t m_nextId;
    std::map<std::string, AIZone*> m_aiZones;

    void AddActorToWorld(Actor *);
    void AddActorToWorldWithId(uint32_t, Actor *);
    void SendEventToAllPlayers(const WriteStream &);
    void SendEventToAllPlayersExcept(Player *, const WriteStream &);
  public:
    World();
    virtual ~World();
    virtual void Tick(float);
    virtual bool HasLocalPlayer();
    ILocalPlayer * GetLocalPlayer();
    virtual bool IsAuthority();
    virtual void AddLocalPlayer(Player *, ILocalPlayer *);
    virtual void AddRemotePlayer(Player *);
    virtual void AddRemotePlayerWithId(uint32_t, Player *);
    virtual void RemovePlayer(Player *);
    virtual void Use(Player *, Actor *);
    virtual void Activate(Player *, IItem *);
    virtual void Reload(Player *);
    virtual void Jump(bool);
    virtual void Sprint(bool);
    virtual void FireRequest(bool);
    virtual void TransitionToNPCState(Player *, const std::string &);
    virtual void BuyItem(Player *, Actor *, IItem *, uint32_t);
    virtual void SellItem(Player *, Actor *, IItem *, uint32_t);
    virtual void Respawn(Player *);
    virtual void Teleport(Player *, const std::string &);
    virtual void Chat(Player *, const std::string &);
    virtual void FastTravel(Player *, const std::string &, const std::string &);
    virtual void SetPvPDesired(Player *, bool);
    virtual void SubmitDLCKey(Player *, const std::string &);
    virtual void SetCircuitInputs(Player *, const std::string &, uint32_t);
    virtual void SendAddItemEvent(Player *, IItem *, uint32_t);
    virtual void SendRemoveItemEvent(Player *, IItem *, uint32_t);
    virtual void SendLoadedAmmoEvent(Player *, IItem *, uint32_t);
    virtual void SendPickedUpEvent(Player *, const std::string &);
    virtual void EquipItem(Player *, uint8_t, IItem *);
    virtual void SetCurrentSlot(Player *, uint8_t);
    virtual void SendEquipItemEvent(Player *, uint8_t, IItem *);
    virtual void SendCurrentSlotEvent(Player *, uint8_t);
    virtual void SetCurrentQuest(Player *, IQuest *);
    virtual void SendSetCurrentQuestEvent(Player *, IQuest *);
    virtual void SendStartQuestEvent(Player *, IQuest *);
    virtual void SendAdvanceQuestToStateEvent(Player *, IQuest *, IQuestState *);
    virtual void SendCompleteQuestEvent(Player *, IQuest *);
    virtual void SendHealthUpdateEvent(Actor *, int32_t);
    virtual void SendManaUpdateEvent(Player *, int32_t);
    virtual void SendCountdownUpdateEvent(Player *, int32_t);
    virtual void SendPvPCountdownUpdateEvent(Player *, bool, int32_t);
    virtual void SendPvPEnableEvent(Player *, bool);
    virtual void SendStateEvent(Actor *, const std::string &, bool);
    virtual void SendTriggerEvent(Actor *, const std::string &, Actor *, bool);
    virtual void SendFireBulletsEvent(Actor *, IItem *, const Vector3 &, uint32_t, float);
    virtual void SendDisplayEvent(Player *, const std::string &, const std::string &);
    virtual void SendNPCConversationStateEvent(Player *, Actor *, const std::string &);
    virtual void SendNPCConversationEndEvent(Player *);
    virtual void SendNPCShopEvent(Player *, Actor *);
    virtual void SendRespawnEvent(Player *, const Vector3 &, const Rotation &);
    virtual void SendTeleportEvent(Actor *, const Vector3 &, const Rotation &);
    virtual void SendRelativeTeleportEvent(Actor *, const Vector3 &);
    virtual void SendReloadEvent(Player *, IItem *, IItem *, uint32_t);
    virtual void SendPlayerJoinedEvent(Player *);
    virtual void SendPlayerLeftEvent(Player *);
    virtual void SendPlayerItemEvent(Player *);
    virtual void SendActorSpawnEvent(Actor *);
    virtual void SendActorDestroyEvent(Actor *);
    virtual void SendExistingPlayerEvent(Player *, Player *);
    virtual void SendExistingActorEvent(Player *, Actor *);
    virtual void SendChatEvent(Player *, const std::string &);
    virtual void SendKillEvent(Player *, Actor *, IItem *);
    virtual void SendCircuitOutputEvent(Player *, const std::string &, uint32_t, const std::vector<std::allocator<bool>> &);
    virtual void SendActorPositionEvents(Player *);
    virtual void SendRegionChangeEvent(Player *, const std::string &);
    virtual void SendLastHitByItemEvent(Player *, IItem *);
    bool SpawnActor(Actor *, const Vector3 &, const Rotation &);
    bool SpawnActorAtNamedLocation(Actor *, const char *);
    void SpawnActorWithId(uint32_t, Actor *, const Vector3 &, const Rotation &);
    void DestroyActor(Actor *);
    void SendSpawnEventsForExistingActors(Player *);
    void AddAIZone(AIZone *);
    AIZone * GetAIZone(const std::string &);
    void OnPlayerEnteredAIZone(const std::string &);
    void OnPlayerLeftAIZone(const std::string &);
    std::vector<IPlayer*> GetPlayersInRadius(const Vector3 &, float);
    std::vector<Projectile*> GetProjectilesInRadius(const Vector3 &, float);
    Actor * GetActorById(uint32_t);
    void RemoveAllActorsExceptPlayer(Player *);
    void ChangeActorId(Player *, uint32_t);
    bool IsPlayerAlreadyConnected(uint32_t);
};

class ClientWorld : public World {
  private:
    ActorRef<IPlayer> m_activePlayer;
    float m_timeUntilNextNetTick;

  public:
    ClientWorld();
    virtual bool HasLocalPlayer();
    virtual bool IsAuthority();
    virtual void AddLocalPlayer(Player *, ILocalPlayer *);
    virtual void Tick(float);
    virtual void Use(Player *, Actor *);
    virtual void Activate(Player *, IItem *);
    virtual void Reload(Player *);
    virtual void Jump(bool);
    virtual void Sprint(bool);
    virtual void FireRequest(bool);
    virtual void TransitionToNPCState(Player *, const std::string &);
    virtual void BuyItem(Player *, Actor *, IItem *, uint32_t);
    virtual void SellItem(Player *, Actor *, IItem *, uint32_t);
    virtual void Respawn(Player *);
    virtual void Teleport(Player *, const std::string &);
    virtual void Chat(Player *, const std::string &);
    virtual void FastTravel(Player *, const std::string &, const std::string &);
    virtual void SetPvPDesired(Player *, bool);
    virtual void SubmitDLCKey(Player *, const std::string &);
    virtual void SetCircuitInputs(Player *, const std::string &, uint32_t);
    virtual void SendAddItemEvent(Player *, IItem *, uint32_t);
    virtual void SendRemoveItemEvent(Player *, IItem *, uint32_t);
    virtual void SendLoadedAmmoEvent(Player *, IItem *, uint32_t);
    virtual void SendPickedUpEvent(Player *, const std::string &);
    virtual void EquipItem(Player *, uint8_t, IItem *);
    virtual void SetCurrentSlot(Player *, uint8_t);
    virtual void SendEquipItemEvent(Player *, uint8_t, IItem *);
    virtual void SendCurrentSlotEvent(Player *, uint8_t);
    virtual void SetCurrentQuest(Player *, IQuest *);
    virtual void SendSetCurrentQuestEvent(Player *, IQuest *);
    virtual void SendStartQuestEvent(Player *, IQuest *);
    virtual void SendAdvanceQuestToStateEvent(Player *, IQuest *, IQuestState *);
    virtual void SendCompleteQuestEvent(Player *, IQuest *);
    virtual void SendHealthUpdateEvent(Actor *, int32_t);
    virtual void SendManaUpdateEvent(Player *, int32_t);
    virtual void SendCountdownUpdateEvent(Player *, int32_t);
    virtual void SendPvPCountdownUpdateEvent(Player *, bool, int32_t);
    virtual void SendPvPEnableEvent(Player *, bool);
    virtual void SendStateEvent(Actor *, const std::string &, bool);
    virtual void SendTriggerEvent(Actor *, const std::string &, Actor *, bool);
    virtual void SendFireBulletsEvent(Actor *, IItem *, const Vector3 &, uint32_t, float);
    virtual void SendDisplayEvent(Player *, const std::string &, const std::string &);
    virtual void SendNPCConversationStateEvent(Player *, Actor *, const std::string &);
    virtual void SendNPCConversationEndEvent(Player *);
    virtual void SendNPCShopEvent(Player *, Actor *);
    virtual void SendRespawnEvent(Player *, const Vector3 &, const Rotation &);
    virtual void SendTeleportEvent(Actor *, const Vector3 &, const Rotation &);
    virtual void SendRelativeTeleportEvent(Actor *, const Vector3 &);
    virtual void SendReloadEvent(Player *, IItem *, IItem *, uint32_t);
    virtual void SendPlayerJoinedEvent(Player *);
    virtual void SendPlayerLeftEvent(Player *);
    virtual void SendPlayerItemEvent(Player *);
    virtual void SendActorSpawnEvent(Actor *);
    virtual void SendActorDestroyEvent(Actor *);
    virtual void SendExistingPlayerEvent(Player *, Player *);
    virtual void SendExistingActorEvent(Player *, Actor *);
    virtual void SendChatEvent(Player *, const std::string &);
    virtual void SendKillEvent(Player *, Actor *, IItem *);
    virtual void SendCircuitOutputEvent(Player *, const std::string &, uint32_t, const std::vector<std::allocator<bool>> &);
    virtual void SendActorPositionEvents(Player *);
    virtual void SendRegionChangeEvent(Player *, const std::string &);
    virtual void SendLastHitByItemEvent(Player *, IItem *);
};