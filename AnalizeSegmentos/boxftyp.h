class Ftype{
	private:
		int major_brand;
		int menor_brand;
		int compatible_brand[];
	public:
		int getMajor_brand();
		int getMenor_brand();
		int* getCompatible_brand();
		void setMenor_brand(int);
		void setMajor_brand(int);
		void setCompatible_brand(int*);
};