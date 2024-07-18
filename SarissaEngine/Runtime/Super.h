#pragma once

namespace Framework
{
	// すべての、オブジェクトにかかわるクラスの基底クラス
	class Super
	{
	public:
		Super();
		~Super();

		void Init();
		void Update();
		void Finalize();
	};
}

