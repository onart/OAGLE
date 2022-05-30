/********************************************************************************
* 2D/3D OpenGL Game Engine
* Copyright 2022 onart@github
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************************/
#ifndef __OA_VERTEX_H__
#define __OA_VERTEX_H__

#include "oaglem.h"

#include <type_traits>
#include <tuple>
#include <map>
#include <string>
#include <vector>
#include <memory>

#define USE_BUMP
#define USE_ANIM

namespace onart {

	template<class A>
	inline constexpr bool isOneOf() {
		return false;
	}

	/// <summary>
	/// 첫 번째 템플릿 인자가 나머지 중 하나와 동일한 경우 참을 리턴합니다.
	/// </summary>
	template <class A, class T1, class... Types>
	inline constexpr bool isOneOf() {
		return std::is_same_v<A, T1> || isOneOf<A, Types...>();
	}

	/// <summary>
	/// 렌더링 파이프라인의 정점입니다.
	/// <para>법선 매핑을 위해서는 USE_BUMP 매크로가 정의되어야 합니다.</para>
	/// <para>관절 애니메이션을 위해서는 USE_ANIM 매크로가 정의되어야 합니다.</para>
	/// </summary>
	struct Vertex
	{
		/// <summary>
		/// 모델 내 정점의 위치입니다.
		/// </summary>
		vec3 pos;
		/// <summary>
		/// 정점에서의 법선입니다.
		/// </summary>
		vec3 norm;
		/// <summary>
		/// 2D 텍스처 좌표입니다.
		/// </summary>
		vec2 tc;
#ifdef USE_BUMP
		/// <summary>
		/// 법선 매핑을 위한 접선 벡터입니다.
		/// </summary>
		vec3 tan;
		/// <summary>
		/// 법선 매핑을 위한 접선 벡터에 수직이 되는 벡터입니다. 
		/// </summary>
		vec3 bitan;
#endif // USE_BUMP
#ifdef USE_ANIM
		/// <summary>
		/// 관절 애니메이션을 위한 뼈의 인덱스입니다.
		/// </summary>
		ivec4 boneIDs = { -1,-1,-1,-1 };
		/// <summary>
		/// 관절 애니메이션을 위한 뼈의 영향력(가중치)입니다.
		/// </summary>
		vec4 boneWeights = { 0,0,0,0 };
#endif // USE_ANIM

	};

	/// <summary>
	/// 속성을 직접 설정할 수 있는 정점 클래스입니다.
	/// 단, 인수로는 (unsigned) char, (unsigned) short, (unsigend) int, float, double과 그에 대한 nvec만 가능합니다.
	/// </summary>
	template<class... T>
	struct CustomVertex;

	/// <summary>
	/// 속성을 직접 설정할 수 있는 정점 클래스입니다.
	/// 단, 인수로는 (unsigned) char, (unsigned) short, (unsigend) int, float, double과 그에 대한 nvec만 가능합니다.
	/// </summary>
	template<class F>
	struct CustomVertex<F> {
		F first;
		/// <summary>
		/// 정점의 멤버에 접근할 수 있습니다. 생성은 초기화 리스트를 통하여 하는 것을 권장합니다.
		/// </summary>
		template<unsigned P> inline constexpr auto& get() { static_assert(P == 0, "Index exceeded."); return first; }
		CustomVertex(const CustomVertex&) = default;
		CustomVertex(CustomVertex&&) = default;
		inline static void enableVA() { enableVA(sizeof(CustomVertex<F>), 0, 0); }
		inline static void enableVA(int stride, unsigned index, size_t offset);
	};

	/// <summary>
	/// 속성을 직접 설정할 수 있는 정점 클래스입니다. 생성은 초기화 리스트를 통하여 하는 것을 권장합니다.
	/// 단, 인수로는 (unsigned) char, (unsigned) short, (unsigend) int, float, double과 그에 대한 nvec만 가능합니다.
	/// </summary>
	template <class F, class... T>
	struct CustomVertex<F, T...> {
		F first;
		CustomVertex<T...> rest;
		/// <summary>
		/// 정점의 P번째(0 base) 멤버에 접근할 수 있습니다. 예를 들어 템플릿 매개변수가 int, float, char인 경우 get＜2＞는 char 부분에 대한 참조(포인터 말고 참조자)를 리턴합니다.
		/// </summary>
		template<unsigned P> inline constexpr auto& get() { static_assert(sizeof...(T) >= P, "Index exceeded."); if constexpr (P == 0) return first; else return rest.get<P - 1>(); }
		CustomVertex(const CustomVertex&) = default;
		CustomVertex(CustomVertex&&) = default;
		inline static void enableVA() { enableVA(sizeof(CustomVertex<F, T...>), 0, 0); }
		inline static void enableVA(int stride, unsigned index, size_t offset) {
			CustomVertex<F>::enableVA(stride, index, offset);
			using thisType = CustomVertex<F, T...>;
			CustomVertex<T...>::enableVA(stride, index + 1, offset + offsetof(thisType, rest));
		}
	};

	using DefaultVertex = CustomVertex<vec3, vec3, vec2, vec3, vec3, ivec4, vec4>;	// position, normal vector, textrue coordinate, tangent, bitangent, bone id, bone weight

	/// <summary>
	/// 기초 모델(메터리얼, 애니메이션 등이 없는 것)의 클래스입니다.
	/// </summary>
	class Mesh {
		friend struct std::default_delete<Mesh>;
		public:
			/// <summary>
			/// 기초 모델(메터리얼, 애니메이션 등이 없는 것)을 이름으로 찾아옵니다. 없는 경우 nullptr를 리턴합니다.
			/// </summary>
			/// <param name="name">불러올 때 직접 정한 이름</param>
			/// <returns>메시의 위치를 가리키는 포인터의 주소(Mesh**)</returns>
			static std::shared_ptr<std::unique_ptr<Mesh>> get(const std::string& name);
			/// <summary>
			/// 기초 모델(메터리얼, 애니메이션 등이 없는 것)을 새로 추가합니다.
			/// 단, 예약된 이름을 사용할 수 없으며 그 외에 이미 있는 이름을 대상으로 시도하면 기존 모델을 덮어씁니다.
			/// 덮어쓰이면 기존의 그 모델을 사용하고 있던 모든 모델이 새로 추가한 것으로 바뀝니다. 성공 여부를 리턴합니다(메모리 부족 혹은 빌트인 이름을 사용한 것이 아니면 성공).
			/// <para>예약된 이름: rect, circ, sphr, clnd, cubo, icubo, (빈 문자열)</para>
			/// </summary>
			static bool add(const std::string& name, const std::vector<Vertex>& v, const std::vector<unsigned>& i);
			/// <summary>
			/// 기초 모델을 메모리에서 제거합니다. 사용 중인 모델은 바로 제거되지 않고 사용이 종료되면서 제거됩니다.
			/// 사용 중이라서 바로 제거되지 않은 모델도 get으로 다시 얻을 수 없습니다.
			/// </summary>
			/// <param name="name">제거하려는 모델 이름</param>
			static void drop(const std::string& name);
			/// <summary>
			/// 사용하고 있지 않은 메시를 모두 메모리에서 제거합니다.
			/// 사용하고 있는 메시는 사용이 끝나는 즉시 메모리에서 회수할지, 그대로 남겨둘지 선택할 수 있습니다.
			/// 빌트인 메시는 제거되지 않습니다.
			/// </summary>
			/// <param name="removeUsing">true인 경우 사용 중인 메시도 사용이 끝나면 바로 메모리에서 회수합니다.</param>
			static void collect(bool removeUsing = false);
			/// <summary>
			/// 정점 배열 오브젝트의 id를 얻습니다.
			/// </summary>
			inline unsigned getID() const { return vao; }
			inline unsigned getVB() const { return vb; }
			inline unsigned getIB() const { return ib; }
			/// <summary>
			/// 정점 배열의 길이를 얻습니다.
			/// </summary>			
			inline unsigned getLength() const { return length; }
			/// <summary>
			/// 직사각형 정점 배열 오브젝트를 생성합니다. 재 호출 시 모델을 처음부터 다시 생성합니다.
			/// 텍스처 이미지는 그대로 직사각형에 들어갑니다.
			/// </summary>
			static void rectModel();
			/// <summary>
			/// 원 정점 배열 오브젝트를 생성하고 리턴합니다. 재 호출 시 모델을 처음부터 다시 생성합니다.
			/// 텍스처 이미지는 정사각형 이미지에 내접하는 원입니다.
			/// </summary>
			static void circleModel();
			/// <summary>
			/// 구 정점 배열 오브젝트를 생성하고 리턴합니다. 재 호출 시 모델을 처음부터 다시 생성합니다.
			/// 텍스처 이미지는 지구본과 세계지도의 관계와 동일합니다.
			/// </summary>
			static void sphereModel();
			/// <summary>
			/// 직육면체 정점 배열 오브젝트를 생성하고 리턴합니다. 재 호출 시 모델을 처음부터 다시 생성합니다.
			/// 텍스처 이미지는 4x3 배열의 가득 찬 직사각형이 있을 때 1행 2열이 상,
			/// 2행은 각각 좌, 전, 우, 후,
			/// 3행 2열은 하 방향입니다.
			/// </summary>
			static void cuboidModel();
			/// <summary>
			/// 직육면체 정점 배열 오브젝트를 생성하고 리턴합니다. 텍스처가 직육면체 내부에서만 보이도록 하는 차이가 있습니다. 재 호출 시 모델을 처음부터 다시 생성합니다.
			/// 텍스처 이미지는 4x3 배열의 가득 찬 직사각형이 있을 때 1행 2열이 상,
			/// 2행은 각각 좌, 전, 우, 후,
			/// 3행 2열은 하 방향입니다.
			/// </summary>
			static void iCuboidModel();
			/// <summary>
			/// 정점 벡터와 인덱스 벡터를 가지고 정점 버퍼 오브젝트를 생성하고 리턴합니다.
			/// </summary>
			/// <param name="v">정점 벡터</param>
			/// <param name="i">인덱스 벡터</param>
			/// <param name="vb">정점 버퍼를 받을 주소</param>
			/// <param name="ib">인덱스 버퍼를 받을 주소</param>
			/// <returns></returns>
			static unsigned createVAO(const std::vector<Vertex>& v, const std::vector<unsigned>& i, unsigned* vb, unsigned* ib);
			/// <summary>
			/// 커스텀 정점 클래스에 대하여 정점 벡터와 인덱스 벡터를 가지고 정점 버퍼 오브젝트를 생성하고 리턴합니다.
			/// </summary>
			/// <param name="v">정점 벡터</param>
			/// <param name="i">인덱스 벡터</param>
			/// <param name="vb">정점 버퍼를 받을 주소</param>
			/// <param name="ib">인덱스 버퍼를 받을 주소</param>
			/// <returns></returns>
			template<class... T>
			inline static unsigned createVAO(const std::vector<CustomVertex<T...>>& v, const std::vector<unsigned>& i, unsigned* vb, unsigned* ib);
			/// <summary>
			/// 정점 버퍼와 인덱스 버퍼를 가지고 정점 버퍼 오브젝트를 생성하고 리턴합니다.
			/// </summary>
			static unsigned createVAO(unsigned vb, unsigned ib);
			inline Mesh() {}
		private:
			inline Mesh(unsigned vb, unsigned ib, unsigned vao, unsigned length) :vb(vb), ib(ib), vao(vao), length(length) {};
			~Mesh();	// 프로그램 종료 전까지 임의로 메시 삭제 불가능

			static unsigned generateVBIBVAO(void* vertData, unsigned* idxData, size_t vSize, size_t icount, unsigned* vb, unsigned* ib);
			static void unbindVAO();

			unsigned vb = 0, ib = 0, vao = 0;
			const unsigned length = 0;

			static std::map<std::string, std::shared_ptr<std::unique_ptr<Mesh>>> list;
	};

	using ppMesh = std::shared_ptr<std::unique_ptr<Mesh>>;	// Mesh 클래스에 대한 이중 포인터입니다.

	void floatAttribPointer(unsigned index, int size, int stride, const void* ptr);
	void doubleAttribPointer(unsigned index, int size, int stride, const void* ptr);
	void int8AttribPointer(unsigned index, int size, int stride, const void* ptr);
	void int16AttribPointer(unsigned index, int size, int stride, const void* ptr);
	void int32AttribPointer(unsigned index, int size, int stride, const void* ptr);
	void uint8AttribPointer(unsigned index, int size, int stride, const void* ptr);
	void uint16AttribPointer(unsigned index, int size, int stride, const void* ptr);
	void uint32AttribPointer(unsigned index, int size, int stride, const void* ptr);

	// 인라인 함수***********************************************************************************
	template<class F>
	void CustomVertex<F>::enableVA(int stride, unsigned index, size_t offset) {
		if constexpr (isOneOf<F, vec2, vec3, vec4>()) {
			floatAttribPointer(index, (int)F::DIM(), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, float, float[1], float[2], float[3], float[4]>()) {
			floatAttribPointer(index, sizeof(F) / sizeof(float), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, dvec2, dvec3, dvec4>()) {
			doubleAttribPointer(index, (int)F::DIM(), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, double, double[1], double[2], double[3], double[4]>()) {
			doubleAttribPointer(index, sizeof(F) / sizeof(double), stride, (void*)offset);
		}
		else if constexpr (isOneOf < F, nvec<2, int8_t>, nvec<3, int8_t>, nvec<4, int8_t>>()) {
			int8AttribPointer(index, (int)F::DIM(), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, int8_t, int8_t[1], int8_t[2], int8_t[3], int8_t[4]>()) {
			int8AttribPointer(index, sizeof(F) / sizeof(int8_t), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, nvec<2, uint8_t>, nvec<3, uint8_t>, nvec<4, uint8_t>>()) {
			uint8AttribPointer(index, (int)F::DIM(), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, uint8_t, uint8_t[1], uint8_t[2], uint8_t[3], uint8_t[4]>()) {
			uint8AttribPointer(index, sizeof(F) / sizeof(uint8_t), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, nvec<2, int16_t>, nvec<3, int16_t>, nvec<4, int16_t>>()) {
			int16AttribPointer(index, (int)F::DIM(), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, int16_t, int16_t[1], int16_t[2], int16_t[3], int16_t[4]>()) {
			int16AttribPointer(index, sizeof(F) / sizeof(int16_t), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, nvec<2, uint16_t>, nvec<3, uint16_t>, nvec<4, uint16_t>>()) {
			uint16AttribPointer(index, (int)F::DIM(), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, uint16_t, uint16_t[1], uint16_t[2], uint16_t[3], uint16_t[4]>()) {
			int16AttribPointer(index, sizeof(F) / sizeof(uint16_t), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, ivec2, ivec3, ivec4>()) {
			int32AttribPointer(index, (int)F::DIM(), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, int32_t, int32_t[1], int32_t[2], int32_t[3], int32_t[4]>()) {
			int32AttribPointer(index, sizeof(F) / sizeof(int32_t), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, uvec2, uvec3, uvec4>()) {
			uint32AttribPointer(index, (int)F::DIM(), stride, (void*)offset);
		}
		else if constexpr (isOneOf<F, uint32_t, uint32_t[1], uint32_t[2], uint32_t[3], uint32_t[4]>()) {
			uint32AttribPointer(index, sizeof(F) / sizeof(uint32_t), stride, (void*)offset);
		}
		else {
			static_assert(
				isOneOf<F,
				float, vec2, vec3, vec4, float[1], float[2], float[3], float[4],
				double, dvec2, dvec3, dvec4, double[1], double[2], double[3], double[4],
				int8_t, nvec<2, int8_t>, nvec<3, int8_t>, nvec<4, int8_t>, int8_t[1], int8_t[2], int8_t[3], int8_t[4],
				uint8_t, nvec<2, uint8_t>, nvec<3, uint8_t>, nvec<4, uint8_t>, uint8_t[1], uint8_t[2], uint8_t[3], uint8_t[4],
				int16_t, nvec<2, int16_t>, nvec<3, int16_t>, nvec<4, int16_t>, int16_t[1], int16_t[2], int16_t[3], int16_t[4],
				uint16_t, nvec<2, uint16_t>, nvec<3, uint16_t>, nvec<4, uint16_t>, uint16_t[1], uint16_t[2], uint16_t[3], uint16_t[4],
				int32_t, ivec2, ivec3, ivec4, int32_t[1], int32_t[2], int32_t[3], int32_t[4],
				uint32_t, uvec2, uvec3, uvec4, uint32_t[1], uint32_t[2], uint32_t[3], uint32_t[4]
				>(), "Can't resolve this attribute: only float, double, signed/unsigned char(not JUST \'char\'), short, int and vectors(only onart::nvec) or standard C arrays(fixed 1~4 dim) of them are possible attribute types");
			//  리빙포인트: isOneOf<F>만 남겨 false를 유발해도 동작은 같으나, 가능 타입을 직접 명시하기 위해 남겨 둠. (false로 하는 경우에는 이 조건에 넘어오지 않아도 반드시 컴파일에 실패)
		}
	}

	template<class... T>
	unsigned Mesh::createVAO(const std::vector<CustomVertex<T...>>& v, const std::vector<unsigned>& i, unsigned* vb, unsigned* ib) {
		unsigned vao = generateVBIBVAO(v.data(), i.data(), sizeof(CustomVertex<T...>) * v.size(), i.size(), vb, ib);
		CustomVertex<T...>::enableVA();
		unbindVAO();
		return vao;
	}
}

#endif // !__OA_VERTEX_H__



