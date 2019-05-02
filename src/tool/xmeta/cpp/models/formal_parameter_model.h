#pragma once

#include <string>
#include <string_view>

#include "base_model.h"
#include "model_types.h"

namespace xlang::xmeta
{
    enum class parameter_semantics
    {
        in,
        ref,
        const_ref,
        out
    };

    struct formal_parameter_model : base_model
    {
        formal_parameter_model() = delete;
        formal_parameter_model(std::string_view const& id, size_t decl_line, std::string_view const& assembly_name, parameter_semantics sem, type_ref&& type) :
            base_model{ id, decl_line, assembly_name },
            m_semantic{ sem },
            m_type{ std::move(type) }
        { }

        auto const& get_semantic() const noexcept
        {
            return m_semantic;
        }

        auto const& get_type() const noexcept
        {
            return m_type;
        }

        // NOTE: fully_qualified_id will be a vector of strings once  we have using directives
        void resolve(std::map<std::string, class_type_semantics> const& symbols, std::string fully_qualified_id)
        {
            if (!m_type.get_semantic().is_resolved())
            {
                std::string ref_name = m_type.get_semantic().get_ref_name();
                std::string symbol = ref_name.find(".") != std::string::npos ? ref_name : fully_qualified_id + "." + ref_name;

                auto iter = symbols.find(symbol);
                if (iter == symbols.end())
                {
                    // Reccord the unresolved type and continue
                }
                else
                {
                    m_type.set_semantic(iter->second);
                }
            }
        }

    private:
        parameter_semantics m_semantic;
        type_ref m_type;
    };
}
