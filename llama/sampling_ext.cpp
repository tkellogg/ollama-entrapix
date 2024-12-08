// TODO: this is a temporary wrapper to allow calling C++ code from CGo
#include "sampling.h"
#include "sampling_ext.h"
#include "json-schema-to-grammar.h"

struct gpt_sampler *gpt_sampler_cinit(
    const struct llama_model *model, struct gpt_sampler_cparams *params)
{
    try
    {
        fprintf(stderr, "[entrapix] cinit: entrapix_enabled=%d threshold=%.2f\n", params->entrapix_enabled, params->entrapix_threshold);

        gpt_sampler_params sparams;
        sparams.top_k = params->top_k;
        sparams.top_p = params->top_p;
        sparams.min_p = params->min_p;
        sparams.tfs_z = params->tfs_z;
        sparams.typ_p = params->typical_p;
        sparams.temp = params->temp;
        sparams.penalty_last_n = params->penalty_last_n;
        sparams.penalty_repeat = params->penalty_repeat;
        sparams.penalty_freq = params->penalty_freq;
        sparams.penalty_present = params->penalty_present;
        sparams.mirostat = params->mirostat;
        sparams.mirostat_tau = params->mirostat_tau;
        sparams.mirostat_eta = params->mirostat_eta;
        sparams.penalize_nl = params->penalize_nl;
        sparams.seed = params->seed;
        sparams.grammar = params->grammar;
        sparams.entrapix_enabled = params->entrapix_enabled;
        sparams.entrapix_threshold = params->entrapix_threshold;
        sparams.entrapix_varent = params->entrapix_varent;

        fprintf(stderr, "[entrapix] cinit: entrapix_enabled=%d threshold=%.2f\n", params->entrapix_enabled, params->entrapix_threshold);
        fprintf(stderr, "[entrapix] cinit: copied to sparams: entrapix_enabled=%d threshold=%.2f\n", sparams.entrapix_enabled, sparams.entrapix_threshold);

        return gpt_sampler_init(model, sparams);
    }
    catch (const std::exception &err)
    {
        return nullptr;
    }
}

void gpt_sampler_cfree(struct gpt_sampler *sampler)
{
    gpt_sampler_free(sampler);
}

void gpt_sampler_creset(struct gpt_sampler *sampler)
{
    gpt_sampler_reset(sampler);
}

llama_token gpt_sampler_csample(
    struct gpt_sampler *sampler,
    struct llama_context *ctx_main,
    int idx)
{
    return gpt_sampler_sample(sampler, ctx_main, idx);
}

bool gpt_sampler_cget_is_trap_set(struct gpt_sampler *sampler)
{
    return gpt_sampler_get_is_trap_set(sampler);
}

void gpt_sampler_caccept(
    struct gpt_sampler *sampler,
    llama_token id,
    bool apply_grammar)
{
    gpt_sampler_accept(sampler, id, apply_grammar);
}

int schema_to_grammar(const char *json_schema, char *grammar, size_t max_len)
{
    try
    {
        nlohmann::json schema = nlohmann::json::parse(json_schema);
        std::string grammar_str = json_schema_to_grammar(schema);
        size_t len = grammar_str.length();
        if (len >= max_len)
        {
            len = max_len - 1;
        }
        strncpy(grammar, grammar_str.c_str(), len);
        return len;
    }
    catch (const std::exception &e)
    {
        strncpy(grammar, "", max_len - 1);
        return 0;
    }
}
