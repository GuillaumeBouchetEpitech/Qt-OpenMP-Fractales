


inline void	Fractale::moveUp()
{
	_settings._Position.y = _settings._Position.y + 0.01 * _settings._radius * 1;
}
inline void	Fractale::moveDown()
{
	_settings._Position.y = _settings._Position.y - 0.01 * _settings._radius * 1;
}
inline void	Fractale::moveLeft()
{
	_settings._Position.x = _settings._Position.x - 0.01 * _settings._radius * 1;
}
inline void	Fractale::moveRight()
{
	_settings._Position.x = _settings._Position.x + 0.01 * _settings._radius * 1;
}



inline void	Fractale::zoomIn()
{
	_settings._radius -= 0.01 * _settings._radius;
}

inline void	Fractale::zoomOut()
{
	_settings._radius += 0.01 * _settings._radius;
}



inline void	Fractale::perturbationUp()
{
	_settings._Perturbation.y = _settings._Perturbation.y + 0.01;
}
inline void	Fractale::perturbationDown()
{
	_settings._Perturbation.y = _settings._Perturbation.y - 0.01;
}
inline void	Fractale::perturbationLeft()
{
	_settings._Perturbation.x = _settings._Perturbation.x - 0.01;
}
inline void	Fractale::perturbationRight()
{
	_settings._Perturbation.x = _settings._Perturbation.x + 0.01;
}


