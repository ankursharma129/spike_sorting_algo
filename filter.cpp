struct FilterInput{
	const float* x;
	size_t inputLength;
	const float* c;
	size_t filterLength;
	float* y;
	size_t outputLength;
}

float* firFilter(FilterInput input){
	const auto* x = input.x;
	const auto* c = input.c;
	auto* y = input.y;

	for (auto i=0u; i< input.outputLength; ++i) {
		y[i] = 0.f;
		for (auto j=0u; j<input.filterLength; ++j) {
			y[i] += x[i+j] * c[j];
		}
	}
	return y;
}