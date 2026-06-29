#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <random>

struct IntComponent {
	int32_t Int1;
	int32_t Int2;
	int32_t Int3;
	int32_t Int4;
	int32_t Int5;
	int32_t Int6;
};

struct FloatComponent {
	float Float1;
	float Float2;
	float Float3;
	float Float4;
	float Float5;
	float Float6;
	float Float7;
};

struct DoubleComponent {
	double Double1;
	double Double2;
	double Double3;
	double Double4;
	double Double5;
	double Double6;
	double Double7;
	double Double8;
};

struct IntArrayComponent {
	std::array<int32_t, 8> IntArray1;
	std::array<int32_t, 12> IntArray2;
	std::array<int32_t, 16> IntArray3;
	std::array<int32_t, 24> IntArray4;
	std::array<int32_t, 32> IntArray5;
	std::array<int32_t, 48> IntArray6;
	std::array<int32_t, 64> IntArray7;
	std::array<int32_t, 80> IntArray8;
	std::array<int32_t, 96> IntArray9;
	std::array<int32_t, 128> IntArray10;
};

struct FloatArrayComponent {
	std::array<float, 8> FloatArray1;
	std::array<float, 10> FloatArray2;
	std::array<float, 14> FloatArray3;
	std::array<float, 20> FloatArray4;
	std::array<float, 28> FloatArray5;
	std::array<float, 36> FloatArray6;
	std::array<float, 44> FloatArray7;
	std::array<float, 56> FloatArray8;
	std::array<float, 72> FloatArray9;
	std::array<float, 96> FloatArray10;
};

struct TextComponent {
	std::array<char, 8> Text1;
	std::array<char, 12> Text2;
	std::array<char, 16> Text3;
	std::array<char, 20> Text4;
	std::array<char, 24> Text5;
	std::array<char, 28> Text6;
};

struct BooleanComponent {
	bool Bool1;
	bool Bool2;
	bool Bool3;
	bool Bool4;
	bool Bool5;
	bool Bool6;
	bool Bool7;
	bool Bool8;
	bool Bool9;
};

struct MixedComponentA {
	int32_t Int1;
	float Float1;
	double Double1;
	std::array<char, 24> Text1;
	bool Bool1;
	int32_t Int2;
	float Float2;
	double Double2;
};

struct MixedComponentB {
	std::array<char, 16> Text1;
	double Double1;
	int32_t Int1;
	bool Bool1;
	float Float1;
	double Double2;
	int32_t Int2;
};

struct MixedComponentC {
	float Float1;
	bool Bool1;
	int32_t Int1;
	std::array<char, 20> Text1;
	double Double1;
	int32_t Int2;
};

struct MixedComponentD {
	double Double1;
	std::array<char, 32> Text1;
	float Float1;
	int32_t Int1;
	bool Bool1;
	double Double2;
	std::array<char, 12> Text2;
	float Float2;
	int32_t Int2;
};

struct MixedComponentE {
	bool Bool1;
	int32_t Int1;
	std::array<char, 10> Text1;
	float Float1;
	double Double1;
};

struct LargeNumericComponent {
	IntComponent Ints;
	FloatComponent Floats;
	DoubleComponent Doubles;
	IntArrayComponent IntArrays;
	FloatArrayComponent FloatArrays;
	std::array<int64_t, 128> ExtraInts;
	std::array<float, 128> ExtraFloats;
	std::array<double, 128> ExtraDoubles;
};

struct LargeMixedComponentA {
	MixedComponentA MixedA;
	MixedComponentB MixedB;
	MixedComponentC MixedC;
	TextComponent Texts;
	BooleanComponent Bools;
	std::array<char, 96> TextBlob1;
	std::array<char, 128> TextBlob2;
	std::array<int32_t, 64> Ints;
	std::array<float, 64> Floats;
	std::array<double, 64> Doubles;
};

struct LargeMixedComponentB {
	MixedComponentD MixedD;
	MixedComponentE MixedE;
	IntArrayComponent IntArrays;
	FloatArrayComponent FloatArrays;
	IntComponent Ints;
	FloatComponent Floats;
	DoubleComponent Doubles;
	std::array<char, 72> TextBlob1;
	std::array<char, 104> TextBlob2;
	std::array<uint8_t, 128> Flags;
};

inline std::mt19937& GetRandomGenerator() {
	static std::mt19937 generator(5);
	return generator;
}

inline void ReseedRandomGenerator(uint32_t seed = 5) {
	GetRandomGenerator().seed(seed);
}

inline int RandomInt(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(GetRandomGenerator());
}

inline std::size_t RandomIndex(std::size_t min, std::size_t max) {
	std::uniform_int_distribution<std::size_t> dist(min, max);
	return dist(GetRandomGenerator());
}

inline float RandomFloat(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(GetRandomGenerator());
}

inline double RandomDouble(double min, double max) {
	std::uniform_real_distribution<double> dist(min, max);
	return dist(GetRandomGenerator());
}

inline bool RandomBool() {
	std::bernoulli_distribution dist(0.5);
	return dist(GetRandomGenerator());
}

template <size_t N>
inline std::array<char, N> RandomString() {
	static_assert(N > 0);
	static constexpr char CHARS[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::uniform_int_distribution<size_t> charDist(0, sizeof(CHARS) - 2); // -2 to skip the null terminator

	std::array<char, N> result{};
	if constexpr (N == 1) {
		return result;
	}

	for (size_t i = 0; i < N; ++i) {
		result[i] = CHARS[charDist(GetRandomGenerator())];
	}
	return result;
}

inline IntComponent CreateRandomIntComponent() {
	return IntComponent{
		RandomInt(0, 100), RandomInt(0, 100), RandomInt(0, 100), RandomInt(0, 100), RandomInt(0, 100),
		RandomInt(0, 100)
	};
}

inline FloatComponent CreateRandomFloatComponent() {
	return FloatComponent{
		RandomFloat(0.0f, 100.0f), RandomFloat(0.0f, 100.0f), RandomFloat(0.0f, 100.0f), RandomFloat(0.0f, 100.0f),
		RandomFloat(0.0f, 100.0f), RandomFloat(0.0f, 100.0f), RandomFloat(0.0f, 100.0f)
	};
}

inline DoubleComponent CreateRandomDoubleComponent() {
	return DoubleComponent{
		RandomDouble(0.0, 100.0), RandomDouble(0.0, 100.0), RandomDouble(0.0, 100.0), RandomDouble(0.0, 100.0),
		RandomDouble(0.0, 100.0), RandomDouble(0.0, 100.0), RandomDouble(0.0, 100.0), RandomDouble(0.0, 100.0)
	};
}

template <size_t N>
inline std::array<int32_t, N> CreateRandomIntArray() {
	std::array<int32_t, N> values{};
	for (size_t i = 0; i < N; ++i) {
		values[i] = RandomInt(0, 100);
	}
	return values;
}

template <size_t N>
inline std::array<float, N> CreateRandomFloatArray() {
	std::array<float, N> values{};
	for (size_t i = 0; i < N; ++i) {
		values[i] = RandomFloat(0.0f, 100.0f);
	}
	return values;
}

template <size_t N>
inline std::array<double, N> CreateRandomDoubleArray() {
	std::array<double, N> values{};
	for (size_t i = 0; i < N; ++i) {
		values[i] = RandomDouble(0.0, 100.0);
	}
	return values;
}

template <size_t N>
inline std::array<int64_t, N> CreateRandomInt64Array() {
	std::array<int64_t, N> values{};
	for (size_t i = 0; i < N; ++i) {
		values[i] = static_cast<int64_t>(RandomInt(0, 100));
	}
	return values;
}

template <size_t N>
inline std::array<uint8_t, N> CreateRandomByteArray() {
	std::array<uint8_t, N> values{};
	for (size_t i = 0; i < N; ++i) {
		values[i] = static_cast<uint8_t>(RandomInt(0, 255));
	}
	return values;
}

inline IntArrayComponent CreateRandomIntArrayComponent() {
	return IntArrayComponent{
		CreateRandomIntArray<8>(),
		CreateRandomIntArray<12>(),
		CreateRandomIntArray<16>(),
		CreateRandomIntArray<24>(),
		CreateRandomIntArray<32>(),
		CreateRandomIntArray<48>(),
		CreateRandomIntArray<64>(),
		CreateRandomIntArray<80>(),
		CreateRandomIntArray<96>(),
		CreateRandomIntArray<128>()
	};
}

inline FloatArrayComponent CreateRandomFloatArrayComponent() {
	return FloatArrayComponent{
		CreateRandomFloatArray<8>(),
		CreateRandomFloatArray<10>(),
		CreateRandomFloatArray<14>(),
		CreateRandomFloatArray<20>(),
		CreateRandomFloatArray<28>(),
		CreateRandomFloatArray<36>(),
		CreateRandomFloatArray<44>(),
		CreateRandomFloatArray<56>(),
		CreateRandomFloatArray<72>(),
		CreateRandomFloatArray<96>()
	};
}

inline TextComponent CreateRandomTextComponent() {
	return TextComponent{
		RandomString<8>(), RandomString<12>(), RandomString<16>(), RandomString<20>(), RandomString<24>(),
		RandomString<28>()
	};
}

inline BooleanComponent CreateRandomBooleanComponent() {
	return BooleanComponent{
		RandomBool(), RandomBool(), RandomBool(), RandomBool(), RandomBool(),
		RandomBool(), RandomBool(), RandomBool(), RandomBool()
	};
}

inline MixedComponentA CreateRandomMixedComponentA() {
	return MixedComponentA{
		RandomInt(0, 100), RandomFloat(0.0f, 100.0f), RandomDouble(0.0, 100.0), RandomString<24>(), RandomBool(),
		RandomInt(0, 100), RandomFloat(0.0f, 100.0f), RandomDouble(0.0, 100.0)
	};
}

inline MixedComponentB CreateRandomMixedComponentB() {
	return MixedComponentB{
		RandomString<16>(), RandomDouble(0.0, 100.0), RandomInt(0, 100), RandomBool(), RandomFloat(0.0f, 100.0f),
		RandomDouble(0.0, 100.0), RandomInt(0, 100)
	};
}

inline MixedComponentC CreateRandomMixedComponentC() {
	return MixedComponentC{
		RandomFloat(0.0f, 100.0f), RandomBool(), RandomInt(0, 100), RandomString<20>(), RandomDouble(0.0, 100.0),
		RandomInt(0, 100)
	};
}

inline MixedComponentD CreateRandomMixedComponentD() {
	return MixedComponentD{
		RandomDouble(0.0, 100.0), RandomString<32>(), RandomFloat(0.0f, 100.0f), RandomInt(0, 100), RandomBool(),
		RandomDouble(0.0, 100.0), RandomString<12>(), RandomFloat(0.0f, 100.0f), RandomInt(0, 100)
	};
}

inline MixedComponentE CreateRandomMixedComponentE() {
	return MixedComponentE{
		RandomBool(), RandomInt(0, 100), RandomString<10>(), RandomFloat(0.0f, 100.0f), RandomDouble(0.0, 100.0)
	};
}