std::deque<std::string>		split(std::string to_split, std::string delimiter)
{
	std::deque<std::string>	result;
	size_t	start, end = delimiter.size();

	do
	{
		start = end + delimiter.size();
		end = to_split.find(delimiter, start);
		if (end != std::string::npos)
			result.push_back(to_split.substr(start, end - start));
	} while (end != std::string::npos);

	return (result);
}

