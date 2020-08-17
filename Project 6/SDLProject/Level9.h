#include "Scene.h"
class Level9 : public Scene {

public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
	void Reset();
};

