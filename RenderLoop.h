#pragma once

#include <QObject>
#include <QThread>


class Graphics;
class StateMachine;

class RenderLoop : public QObject
{
	Q_OBJECT

public:
	RenderLoop(QObject *parent = nullptr);
	~RenderLoop();
	void Stop();

	void Zoom(float delta);

private:
	float scale;
	StateMachine * sys;
	bool shouldClosed = false;

signals:
	void Vsync(unsigned char *image, const unsigned int &num_triangles,
		const unsigned int &num_vertices);

public slots:
	void MainLoop();
};
