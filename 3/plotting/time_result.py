class TimeResult:
    def __init__(self, function_name, time_values):
        self.__function_name = function_name
        self.__time_values   = time_values

    def get_time_values(self):
        return self.__time_values

    def get_function_name(self):
        return self.__function_name