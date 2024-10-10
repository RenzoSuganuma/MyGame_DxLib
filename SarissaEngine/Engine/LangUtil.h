#pragma once

// C#の自動プロパティを再現。左から型、プロパティ名、本体の変数
#define AutoProperty( type , name , rawValue ) \
public : const type& get##name() { return rawValue; } \
void set##name( const type& value ) { rawValue = value; }

// C#のプロパティを再現。左から型、変数名（public）、値を受け渡す前の前処理、値を初期化する前の前処理
#define Property( type , name , getterbody , setterbody ) \
public: type name; \
const type& get##name() { getterbody return name; } \
void set##name(const type& val) { setterbody name = val; }
