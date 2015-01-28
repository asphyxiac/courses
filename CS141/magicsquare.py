def helper(x, min_len, a, b, c):
	if len(x) < min_len:
		return True
	return x[a] + x[b] + x[c] == 15

def isValid(x):
	return helper(x, 3, 0, 1, 2) &&
		   helper(x, 6, 3, 4, 5) &&
		   helper(x, 9, 6, 7, 8) &&
		   helper(x, 7, 0, 3, 6) && 
		   helper(x, 8, 1, 4, 7) &&
		   helper(x, 9, 2, 5, 8) &&
		   helper(x, 7, 2, 4, 6) &&
		   helper(x, 9, 0, 4, 8)

def assign(assigned):
	for next in range(1,10):
		if next in assigned:
			continue
		longer = assigned + [next]
		if not isValid(longer):
			return False
		found = assign(longer)
		if not found:
			continue
		return longer
	return True